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
                                       MachineBasicBlock &MBB) const {}
void MyriscFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}
void MyriscFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                               BitVector &SavedRegs,
                                               RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
}
bool llvm::MyriscFrameLowering::hasFP(const MachineFunction &MF) const {
  return false;
}
uint64_t MyriscFrameLowering::computeStateSize(MachineFunction &MF) const {
  return 0;
}