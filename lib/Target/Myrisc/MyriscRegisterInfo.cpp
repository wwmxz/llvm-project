//
// Created by 10587 on 2026/3/5.
//

#include "MyriscRegisterInfo.h"



#include "Myrisc.h"
#include "MyriscSubtarget.h"
#include "MyriscRegisterInfo.h"
// #include "MyriscMachineFunctionInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/STLExtras.h"

#define GET_REGINFO_TARGET_DESC
#include "MyriscGenRegisterInfo.inc"

#define DEBUG_TYPE "myrisc-reg-info"

using namespace llvm;

MyriscRegisterInfo::MyriscRegisterInfo(const MyriscSubtarget &ST)
  : MyriscGenRegisterInfo(Myrisc::RA), Subtarget(ST) {}

//===----------------------------------------------------------------------===//
// Callee Saved Registers methods
//===----------------------------------------------------------------------===//
/// Myrisc Callee Saved Registers
// In MyriscCallConv.td, defined CalleeSavedRegs
const MCPhysReg *
MyriscRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

const uint32_t *
MyriscRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const {
  return CSR_RegMask;
}

BitVector MyriscRegisterInfo::
getReservedRegs(const MachineFunction &MF) const {
  static const unsigned ReservedCPURegs[] = {
    Myrisc::ZERO, Myrisc::RA, Myrisc::SP
  };
  BitVector Reserved(getNumRegs());
  size_t NumReservedRegs = array_lengthof(ReservedCPURegs);
  // size_t NumReservedRegs = 0;
  for (unsigned I = 0; I < NumReservedRegs; ++I)
    Reserved.set(ReservedCPURegs[I]);

  return Reserved;
}

bool MyriscRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const {
  return false;
}

bool MyriscRegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const {
  return false;
}

void MyriscRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                         int SPAdj, unsigned FIOperandNum,
                         RegScavenger *RS ) const {
  MachineInstr &MI = *II;
  LLVM_DEBUG(errs() << MI);

  unsigned I = 0;
  while (!MI.getOperand(I).isFI()) {
    ++I;
    assert(I < MI.getNumOperands());
  }

  const int FI = MI.getOperand(I).getIndex();

  /// 根据 index -> 函数栈帧内部的偏移量
  const MachineFunction &MF = *MI.getParent()->getParent();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  int64_t Offset = MFI.getObjectOffset(FI);
  uint64_t STACKSIZE =
      ROUND_UP(MFI.getStackSize(), Subtarget.getFrameLowering()->getStackAlignment());
  Offset += static_cast<int64_t>(STACKSIZE);
  // 加上指令内的常量偏移（SelectAddrFI的Offset）,相对与FI的偏移量
  int64_t O = MI.getOperand(I + 1).getImm();
  Offset += O;
  MI.getOperand(I).ChangeToRegister(Myrisc::SP, false);
  MI.getOperand(I + 1).ChangeToImmediate(Offset);
}

/// Debug information queries.
Register MyriscRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Myrisc::SP;
}

///  Return GPR register class.
const TargetRegisterClass *MyriscRegisterInfo::intRegClass(unsigned Size) {
  return &Myrisc::GPRRegClass;
}