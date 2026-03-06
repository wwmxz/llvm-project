//
// Created by 10587 on 2026/3/6.
//

#include "MyriscSelDAGToDAG.h"



#define DEBUG_TYPE "myrisc-isel"

using namespace llvm;

bool MyriscDAGToDAGISel::runOnMachineFunction(MachineFunction &MF)  {
  Subtarget = &MF.getSubtarget<MyriscSubtarget>();
  bool Ret = SelectionDAGISel::runOnMachineFunction(MF);
  return Ret;
}

bool MyriscDAGToDAGISel::SelectAddrFI(SDNode *Parent, SDValue AddrFI, SDValue &Base, SDValue &Offset) {
  // errs() << "=== Entering SelectAddrFI for: " << "\n";
  // 数组内元素的栈位置区分依赖 Offset，相对与FI的偏移量
  if (CurDAG->isBaseWithConstantOffset(AddrFI)) {
    // errs() << "=== Entering BaseWithConstantOffset for: " << "\n";
    ConstantSDNode* CN=dyn_cast<ConstantSDNode>(AddrFI.getOperand(1));
    if (FrameIndexSDNode* FIN=dyn_cast<FrameIndexSDNode>(AddrFI.getOperand(0))) {
      // errs() << "=== Entering FrameIndexSDNode for: " << "\n";
      Base=CurDAG->getTargetFrameIndex(FIN->getIndex(),AddrFI.getValueType());
    }
    else {// 场景2：基地址不是FrameIndex → 直接用原基地址（不是栈帧索引，而是寄存器、全局变量符号等）
      // errs() << "=== Entering No FrameIndex for: " << "\n";
      Base=AddrFI.getOperand(0);
    }
    Offset=CurDAG->getTargetConstant(CN->getZExtValue(),SDLoc(AddrFI),AddrFI.getValueType());
    return true;
  }
  if (FrameIndexSDNode* FIN=dyn_cast<FrameIndexSDNode>(AddrFI)) {
    Base=CurDAG->getTargetFrameIndex(FIN->getIndex(),AddrFI.getValueType());
    Offset=CurDAG->getTargetConstant(0,SDLoc(AddrFI),AddrFI.getValueType());
    errs() << "=== Entering  FrameIndex for Offset: " << "\n";
    return true;
  }

  return false;
}


void MyriscDAGToDAGISel::Select(SDNode *Node) {
  if (Node->isMachineOpcode()) {
    Node->setNodeId(-1);
    return;
  }
  SDLoc DL(Node);

  LLVM_DEBUG(dbgs() << "Selecting: "; Node->dump(CurDAG); dbgs() << '\n');

  SelectCode(Node);
}

FunctionPass *llvm::createMyriscISelDag(MyriscTargetMachine &TM,CodeGenOpt::Level OptLevel) {
  return new MyriscDAGToDAGISel(TM,OptLevel);
}