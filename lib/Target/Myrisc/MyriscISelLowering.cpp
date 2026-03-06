//
// Created by 10587 on 2026/3/5.
//

#include "MyriscISelLowering.h"

#include "MCTargetDesc/MyriscMCExpr.h"
#include "MyriscSubtarget.h"
#include "MyriscTargetMachine.h"

#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegionInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "myrisc-lower"

#include "MyriscGenCallingConv.inc"


MyriscTargetLowering::MyriscTargetLowering(const TargetMachine &TM,
                                           const MyriscSubtarget &STI)
  :TargetLowering(TM),STI(STI) {
  /// 注册RegiserClass
  /// 还要处理合法化（类型和操作）
  addRegisterClass(MVT::i32, &Myrisc::GPRRegClass);

  /// 注册合法化的操作
  setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
  setOperationAction(ISD::BR_CC, MVT::i32, Expand);

  // deirved properties we expose.
  computeRegisterProperties(STI.getRegisterInfo());

}
SDValue
MyriscTargetLowering::LowerCall(CallLoweringInfo &CLI,
                                SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &DL = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  /// caller输入的值，即输出给callee的值（实参）
  ///
  ///
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  CallingConv::ID CallConv = CLI.CallConv;
  bool IsVarArg = CLI.IsVarArg;

  /// 1、按照CallConv，确定实参传递的位置，寄存器、栈
  ///
  ///
  MachineFunction &MF = DAG.getMachineFunction();
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeCallOperands(Outs, CC_Myrisc);
  /// 寄存器存储信息 寄存器-值
  SmallVector<std::pair<unsigned, SDValue>> RegsPairs;
  /// 栈存储信息：栈基地址
  SDValue StackPtr;

  for (unsigned i=0,e=ArgLocs.size(); i!=e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    if (VA.isRegLoc()) {
      RegsPairs.push_back(std::make_pair(VA.getLocReg(), OutVals[i]));
    }
    else {
      // store节点去放置实参
      assert(VA.isMemLoc());
      if (!StackPtr.getNode()) {
        StackPtr = DAG.getCopyFromReg(Chain, DL, Myrisc::SP,
                                      getPointerTy(DAG.getDataLayout()));
      }
      unsigned LocMemOffset = VA.getLocMemOffset();
      SDValue PtrOff = DAG.getIntPtrConstant(LocMemOffset, DL);
      PtrOff = DAG.getNode(ISD::ADD, DL, getPointerTy(DAG.getDataLayout()),
                           StackPtr, PtrOff);
      /// store val -> reg + offset
      Chain = DAG.getStore(Chain, DL, OutVals[i], PtrOff,
                           MachinePointerInfo::getStack(MF, LocMemOffset));
    }
  }
  ///2、加载函数地址，全局地址32位，需要HI和LO存储
  ///
  ///
  GlobalAddressSDNode *N = dyn_cast<GlobalAddressSDNode>(Callee);
  if (N!=nullptr) {
    MVT Ty = getPointerTy(DAG.getDataLayout());
    SDValue Hi =
        DAG.getTargetGlobalAddress(N->getGlobal(), DL, Ty, 0, MyriscMCExpr::HI);
    SDValue Lo =
        DAG.getTargetGlobalAddress(N->getGlobal(), DL, Ty, 0, MyriscMCExpr::LO);

    SDValue MHiNode = SDValue(DAG.getMachineNode(Myrisc::LUI, DL, Ty, Hi), 0);
    Callee = SDValue(DAG.getMachineNode(Myrisc::ADDI, DL, Ty, MHiNode, Lo), 0);
  }
  else if (ExternalSymbolSDNode *S = dyn_cast<ExternalSymbolSDNode>(Callee)) {
    MVT Ty = getPointerTy(DAG.getDataLayout());
    SDValue Hi = DAG.getTargetExternalSymbol(S->getSymbol(), Ty, MyriscMCExpr::HI);
    SDValue Lo = DAG.getTargetExternalSymbol(S->getSymbol(), Ty, MyriscMCExpr::LO);

    SDValue MHiNode = SDValue(DAG.getMachineNode(Myrisc::LUI, DL, Ty, Hi), 0);
    Callee = SDValue(DAG.getMachineNode(Myrisc::ADDI, DL, Ty, MHiNode, Lo), 0);
  }

  /// 3、生成CALLNode
  ///
  ///

  /// Ops 列表：Chain（依赖链） + Callee（函数地址） + 传参寄存器 + 寄存器掩码 + Glue
  ///
  ///
  SmallVector<SDValue, 8> Ops(1, Chain);
  Ops.push_back(Callee);

  SDValue Glue;

  /// 传参到寄存器
  ///
  ///
  for (int i = 0, e = RegsPairs.size(); i != e; ++i) {
    unsigned reg = RegsPairs[i].first;
    SDValue& val= RegsPairs[i].second;
    Chain = DAG.getCopyToReg(Chain, DL, reg, val, Glue);
    Glue = Chain.getValue(1);
    Ops.push_back(DAG.getRegister(reg, val.getValueType()));
  }
  /// 确定保留寄存器掩码，告诉 LLVM 哪些寄存器调用后不变
  ///
  ///
  const TargetRegisterInfo *TRI = STI.getRegisterInfo();
  const uint32_t *Mask =
      TRI->getCallPreservedMask(DAG.getMachineFunction(), CallConv);
  Ops.push_back(DAG.getRegisterMask(Mask));
  if (Glue.getNode()) {
    Ops.push_back(Glue);
  }

  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  ///生成调用节点
  Chain = DAG.getNode(MyriscISD::CALL, DL, NodeTys, Ops);

  {
    /// 4、处理返回值：分析返回值位置从返回寄存器读取值，存入InVals
    ///
    ///
    SDValue Glue = Chain.getValue(1);
    SmallVector<CCValAssign, 2> RVLos;
    CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLos,
                   *DAG.getContext());
    CCInfo.AnalyzeCallResult(Ins, RetCC_Myrisc);

    for (unsigned i = 0, e = RVLos.size(); i != e; ++i) {
      CCValAssign &VA = RVLos[i];
      EVT vt = RVLos[i].getLocVT();
      assert(VA.isRegLoc());
      unsigned RVReg = VA.getLocReg();
      SDValue Val = DAG.getCopyFromReg(Chain, DL, RVReg, vt, Glue);
      Chain = Val.getValue(1);
      Glue = Val.getValue(2);
      InVals.push_back(Val);
    }
  }

  return Chain;
}
SDValue MyriscTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  // return Chain;
  MachineFunction& MF=DAG.getMachineFunction();
  MachineFrameInfo& MFI=MF.getFrameInfo();
  ///根据调用约定，分析所传参数所在pos(寄存器、栈）
  ///
  ///
  SmallVector<CCValAssign,16> ArgLocs;
  CCState CCInfo(CallConv,IsVarArg,MF,ArgLocs,*DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins,CC_Myrisc);

  ///从寄存器、栈中拷贝值到invals，便于callee访问
  ///
  ///
  SDValue ArgValue;
  for (unsigned i=0,e=ArgLocs.size(); i!=e; ++i) {
    CCValAssign& VA=ArgLocs[i];
    ///激活寄存器从中拷贝
    if (VA.isRegLoc()) {
      MVT RegVT=VA.getValVT();
      Register Reg=MF.addLiveIn(VA.getLocReg(),&Myrisc::GPRRegClass);
      ArgValue=DAG.getCopyFromReg(Chain,DL,Reg,RegVT);
      InVals.push_back(ArgValue);
    }
    else {///创建栈的偏移对象，然后使用 load 节点来加载值
      assert(VA.isMemLoc() && "Myrisc architecture only supports memory arguments for stack!");
      MVT ValVT=VA.getValVT();
      int offset=VA.getLocMemOffset();
      int FI=MFI.CreateFixedObject(ValVT.getSizeInBits()/8,offset,true);
      SDValue FIN = DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
      SDValue Val = DAG.getLoad(
          ValVT, DL, Chain, FIN,
          MachinePointerInfo::getFixedStack(DAG.getMachineFunction(), FI));
      InVals.push_back(Val);

    }
  }

  return Chain;
}

SDValue
MyriscTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::OutputArg> &Outs,
                               const SmallVectorImpl<SDValue> &OutVals,
                               const SDLoc &DL, SelectionDAG &DAG) const {
  // SmallVector<SDValue, 4> RetOps(1, Chain);
  // return DAG.getNode(MyriscISD::RET_GLUE, DL, MVT::Other,RetOps);
  SmallVector<CCValAssign, 16> RVLocs;

  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeReturn(Outs, RetCC_Myrisc);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);
  ///将返回值传入寄存器中
  ///
  ///
  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain;

  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  return DAG.getNode(MyriscISD::RET_GLUE, DL, MVT::Other, RetOps);
}
SDValue MyriscTargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
  default:
    llvm::llvm_unreachable_internal("unknown op");
  }
  return SDValue();
}

const char *MyriscTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case MyriscISD::RET_GLUE:
    return "MyriscISD::RET_GLUE";
  case MyriscISD::CALL:
    return "MyriscISD::CALL";
  case MyriscISD::HI:
    return "MyriscISD::HI";
  case MyriscISD::LO:
    return "MyriscISD::LO";
  default:
    return nullptr;
  }
}
SDValue MyriscTargetLowering::LowerGlobalAddress(SDValue Op,
                                                 SelectionDAG &DAG) const {
  EVT VT = Op.getValueType();
  GlobalAddressSDNode *N = dyn_cast<GlobalAddressSDNode>(Op);
  int64_t Offset = N->getOffset();
  SDLoc DL(N);
  SDValue Hi =
      DAG.getTargetGlobalAddress(N->getGlobal(), DL, VT, 0, MyriscMCExpr::HI);
  SDValue Lo =
      DAG.getTargetGlobalAddress(N->getGlobal(), DL, VT, 0, MyriscMCExpr::LO);

  SDValue MHiNode = SDValue(DAG.getMachineNode(Myrisc::LUI, DL, VT, Hi), 0);
  SDValue BaseAddr =
      SDValue(DAG.getMachineNode(Myrisc::ADDI, DL, VT, MHiNode, Lo), 0);
  if (Offset) {
    return DAG.getNode(ISD::ADD, DL, VT, BaseAddr,
                       DAG.getConstant(Offset, DL, VT));
  }
  return BaseAddr;
}
SDValue MyriscTargetLowering::LowerConstant(SDValue Op,
                                            SelectionDAG &DAG) const {
  return SDValue();
}