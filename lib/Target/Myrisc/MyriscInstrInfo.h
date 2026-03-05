//
// Created by 10587 on 2026/3/5.
//

#ifndef LLVM_MYRISCINSTRINFO_H
#define LLVM_MYRISCINSTRINFO_H

#include "Myrisc.h"
#include "MyriscRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "MyriscGenInstrInfo.inc"

namespace llvm {
class MyriscSubtarget;
class MyriscInstrInfo : public MyriscGenInstrInfo {

protected:
  const MyriscSubtarget & STI;
public:
  explicit MyriscInstrInfo(const MyriscSubtarget & STI);

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                  const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                  bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator MI,
                                   Register SrcReg, bool isKill, int FrameIndex,
                                   const TargetRegisterClass *RC,
                                   const TargetRegisterInfo *TRI) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                                    MachineBasicBlock::iterator MI,
                                    Register DestReg, int FrameIndex,
                                    const TargetRegisterClass *RC,
                                    const TargetRegisterInfo *TRI) const override;

  // Adjust SP by Amount bytes.
  void adjustStackPtr(unsigned SP, int64_t Amount, MachineBasicBlock &MBB,
                      MachineBasicBlock::iterator I) const ;

  // Emit a series of instructions to load an immediate. If NewImm is a
  // non-NULL parameter, the last instruction is not emitted, but instead
  // its immediate operand is returned in NewImm.
  unsigned loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
                         MachineBasicBlock::iterator II, const DebugLoc &DL,
                         unsigned *NewImm) const;

};
}


#endif // LLVM_MYRISCINSTRINFO_H
