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
  /// add dst, zero, src
  MachineInstrBuilder MIB = BuildMI(MBB, MI, DL, get(Myrisc::ADD));
  MIB.addReg(DestReg, RegState::Define);
  MIB.addReg(Myrisc::ZERO);
  MIB.addReg(SrcReg, getKillRegState(KillSrc));
}
void MyriscInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MI,
                                          Register SrcReg, bool isKill,
                                          int FrameIndex,
                                          const TargetRegisterClass *RC,
                                          const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  BuildMI(MBB, MI, DL, get(Myrisc::STOREWFI))
      .addReg(SrcReg, getKillRegState(isKill))
      .addFrameIndex(FrameIndex)
      .addImm(0);
}
void MyriscInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register DestReg,
    int FrameIndex, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  BuildMI(MBB, MI, DL, get(Myrisc::LOADWFI), DestReg)
      .addFrameIndex(FrameIndex)
      .addImm(0);
}
void MyriscInstrInfo::adjustStackPtr(unsigned SP, int64_t Amount,
                                     MachineBasicBlock &MBB,
                                     MachineBasicBlock::iterator I) const {

}
unsigned MyriscInstrInfo::loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator II,
                                        const DebugLoc &DL,
                                        unsigned *NewImm) const {
  return 0;
}