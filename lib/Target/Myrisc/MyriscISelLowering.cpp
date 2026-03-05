//
// Created by 10587 on 2026/3/5.
//

#include "MyriscISelLowering.h"


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

  // deirved properties we expose.
  computeRegisterProperties(STI.getRegisterInfo());

}
SDValue
MyriscTargetLowering::LowerCall(CallLoweringInfo &CLI,
                                SmallVectorImpl<SDValue> &InVals) const {
  return TargetLowering::LowerCall(CLI, InVals);
}
SDValue MyriscTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  return TargetLowering::LowerFormalArguments(Chain, CallConv, IsVarArg, Ins,
                                              DL, DAG, InVals);
}

SDValue
MyriscTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::OutputArg> &Outs,
                               const SmallVectorImpl<SDValue> &OutVals,
                               const SDLoc &DL, SelectionDAG &DAG) const {

  return Chain;
}
SDValue MyriscTargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {
  return TargetLowering::LowerOperation(Op, DAG);
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
  return SDValue();
}
SDValue MyriscTargetLowering::LowerConstant(SDValue Op,
                                            SelectionDAG &DAG) const {
  return SDValue();
}