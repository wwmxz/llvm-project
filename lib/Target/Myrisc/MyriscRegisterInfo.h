//
// Created by 10587 on 2026/3/5.
//

#ifndef LLVM_MYRISCREGISTERINFO_H
#define LLVM_MYRISCREGISTERINFO_H

#include "Myrisc.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "MyriscGenRegisterInfo.inc"

namespace llvm {
class MyriscSubtarget;
class TargetInstrInfo;
class Type;

class MyriscRegisterInfo : public MyriscGenRegisterInfo {
protected:
  const MyriscSubtarget &Subtarget;

public:
  MyriscRegisterInfo(const MyriscSubtarget &Subtarget);

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool requiresRegisterScavenging(const MachineFunction &MF) const override;

  bool trackLivenessAfterRegAlloc(const MachineFunction &MF) const override;

  /// Stack Frame Processing Methods
  void eliminateFrameIndex(MachineBasicBlock::iterator II,
                           int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  /// Debug information queries.
  Register getFrameRegister(const MachineFunction &MF) const override;

  ///  Return GPR register class.
  static const TargetRegisterClass *intRegClass(unsigned Size);
};

} // end namespace llvm

#endif // LLVM_MYRISCREGISTERINFO_H
