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
  DebugLoc DL = I != MBB.end() ? I->getDebugLoc() : DebugLoc();
  unsigned ADDu = Myrisc::ADD;
  unsigned ADDiu = Myrisc::ADDI;

  if (isInt<12>(Amount)) {
    // addiu sp, sp, amount
    BuildMI(MBB, I, DL, get(ADDiu), SP).addReg(SP).addImm(Amount);
  }
  else { // Expand immediate that doesn't fit in 12-bit.
    unsigned Reg = loadImmediate(Amount, MBB, I, DL, nullptr);
    BuildMI(MBB, I, DL, get(ADDu), SP).addReg(SP).addReg(Reg, RegState::Kill);
  }
}
unsigned MyriscInstrInfo::loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator II,
                                        const DebugLoc &DL,
                                        unsigned *NewImm) const {
  unsigned T0 = Myrisc::T0;
  unsigned LUI = Myrisc::LUI;
  unsigned ADDI = Myrisc::ADDI;

  // 步骤 1：将 32 位有符号立即数拆分为高 20 位 + 低 12 位
  // RISC-V LUI 加载高 20 位（左移 12 位），ADDI 补充低 12 位（带符号）
  uint32_t Imm32 = static_cast<uint32_t>(Imm); // 截断为 32 位
  uint32_t Hi20 = (Imm32 >> 12) & 0xFFFFF;   // 高 20 位（bit31~bit12）
  uint32_t Lo12 = Imm32 & 0xFFF;             // 低 12 位（bit11~bit0）

  // 步骤 2：处理低 12 位的符号扩展（若低 12 位最高位为 1，Hi20 需 +1）
  // 原因：ADDI 的 12 位是有符号数，Lo12 >= 0x800 时会符号扩展为负数
  if (Lo12 & 0x800) {
    Hi20 += 1;
  }

  // 步骤 3：生成 LUI 指令（加载高 20 位到 T0）
  // LUI T0, Hi20 → T0 = Hi20 << 12
  BuildMI(MBB, II, DL, get(LUI), T0)
    .addImm(static_cast<int64_t>(Hi20)); // 传入 Hi20（自动转补码）

  // 步骤 4：生成 ADDI 指令（补充低 12 位）
  // ADDI T0, T0, Lo12 → T0 = (Hi20 << 12) + Lo12（带符号）
  // 注意：Lo12 需转成 12 位有符号数（-2048 ~ 2047）
  int64_t Lo12Signed = SignExtend64<12>(Lo12);
  BuildMI(MBB, II, DL, get(ADDI), T0)
    .addReg(T0)
    .addImm(Lo12Signed);

  // 返回临时寄存器 T0（已加载完整的 Imm）
  return T0;
}