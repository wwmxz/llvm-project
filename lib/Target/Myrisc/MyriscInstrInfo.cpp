//
// Created by 10587 on 2026/3/5.
//

#include "MyriscInstrInfo.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "MyriscGenInstrInfo.inc"

using namespace llvm;

MyriscInstrInfo::MyriscInstrInfo(const MyriscSubtarget & STI) : STI(STI) {}
void MyriscInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MI,
                                  const DebugLoc &DL, MCRegister DestReg,
                                  MCRegister SrcReg, bool KillSrc) const {
  MyriscGenInstrInfo::copyPhysReg(MBB, MI, DL, DestReg, SrcReg, KillSrc);
}
void MyriscInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MI,
                                          Register SrcReg, bool isKill,
                                          int FrameIndex,
                                          const TargetRegisterClass *RC,
                                          const TargetRegisterInfo *TRI) const {
  MyriscGenInstrInfo::storeRegToStackSlot(MBB, MI, SrcReg, isKill, FrameIndex,
                                          RC, TRI);
}
void MyriscInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register DestReg,
    int FrameIndex, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI) const {
  MyriscGenInstrInfo::loadRegFromStackSlot(MBB, MI, DestReg, FrameIndex, RC,
                                           TRI);
}
void MyriscInstrInfo::adjustStackPtr(unsigned SP, int64_t Amount,
                                     MachineBasicBlock &MBB,
                                     MachineBasicBlock::iterator I) const {}
unsigned MyriscInstrInfo::loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator II,
                                        const DebugLoc &DL,
                                        unsigned *NewImm) const {
  return 0;
}