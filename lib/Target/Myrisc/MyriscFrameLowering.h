//
// Created by 10587 on 2026/3/5.
//

#ifndef LLVM_MYRISCFRAMELOWERING_H
#define LLVM_MYRISCFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm{
class MyriscSubtarget;
///栈帧管理器：栈的生长方向、对齐数、序言尾声的生成等
///
///
///
class MyriscFrameLowering : public TargetFrameLowering {
  const MyriscSubtarget &STI;

public:
  explicit MyriscFrameLowering(const MyriscSubtarget &STI);
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs, RegScavenger *RS) const override;

  bool hasFP(const MachineFunction &MF) const override;

private:
  uint64_t computeStateSize(MachineFunction &MF)const;
};


}


#endif // LLVM_MYRISCFRAMELOWERING_H
