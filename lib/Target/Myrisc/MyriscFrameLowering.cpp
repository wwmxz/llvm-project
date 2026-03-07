//
// Created by 10587 on 2026/3/5.
//

#include "MyriscFrameLowering.h"

#include "MyriscSubtarget.h"

using namespace llvm;

MyriscFrameLowering::MyriscFrameLowering(const MyriscSubtarget &STI)
: TargetFrameLowering(TargetFrameLowering::StackGrowsDown,
      Align(STI.stackAlignment()), 0,Align(STI.stackAlignment())),
        STI(STI) {

}
void MyriscFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");
  MachineFrameInfo &MFI    = MF.getFrameInfo();

  const MyriscInstrInfo &TII =
    *static_cast<const MyriscInstrInfo*>(STI.getInstrInfo());
  const MyriscRegisterInfo &RegInfo =
    *static_cast<const MyriscRegisterInfo*>(STI.getRegisterInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  // DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  unsigned SP = Myrisc::SP;

  // First, compute final stack size.
  uint64_t StackSize = MFI.getStackSize();

  // No need to allocate space on the stack.
  if (StackSize == 0 && !MFI.adjustsStack()) return;

  // MachineModuleInfo &MMI = MF.getMMI();

  // Adjust stack.
  // DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  // BuildMI(MBB, MBBI, DL, TII.get(Myrisc::ADDI), Myrisc::SP)
  //     .addReg(SP)
  //     .addImm(-StackSize)
  //     .setMIFlag(MachineInstr::FrameSetup);

  // Adjust stack.
  TII.adjustStackPtr(SP, -StackSize, MBB, MBBI);

}
// 生成尾声：
/*
 *
 *
 还原sp
 *
 */
void MyriscFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  const MyriscInstrInfo &TII =
    *static_cast<const MyriscInstrInfo*>(STI.getInstrInfo());
  const MyriscRegisterInfo &RegInfo =
    *static_cast<const MyriscRegisterInfo*>(STI.getRegisterInfo());

  unsigned SP = Myrisc::SP;

  // Get the number of bytes from FrameInfo
  uint64_t StackSize = MFI.getStackSize();

  if (!StackSize)
    return;

  // Adjust stack.
  // DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  // BuildMI(MBB, MBBI, DL, TII.get(Myrisc::ADDI), Myrisc::SP)
  //     .addReg(SP)
  //     .addImm(StackSize)
  //     .setMIFlag(MachineInstr::FrameDestroy);

  // Adjust stack.
  TII.adjustStackPtr(SP, StackSize, MBB, MBBI);
}
void MyriscFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                               BitVector &SavedRegs,
                                               RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  if (MF.getFrameInfo().hasCalls()) {
    SavedRegs.set(Myrisc::RA);
  }
}
bool llvm::MyriscFrameLowering::hasFP(const MachineFunction &MF) const {
  return false;
}
uint64_t MyriscFrameLowering::computeStateSize(MachineFunction &MF) const {
  uint64_t STACKSIZE = MF.getFrameInfo().getStackSize();
  if (getStackAlignment() > 0) {
    STACKSIZE = ROUND_UP(STACKSIZE, getStackAlignment());
  }
  return STACKSIZE;
}