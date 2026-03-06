//
// Created by 10587 on 2026/3/6.
//

#include "MyriscAsmPrinter.h"


#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/TargetRegistry.h"

#define DEBUG_TYPE "Myrisc-asm-printer"


using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMyriscAsmPrinter() {
  RegisterAsmPrinter<MyriscAsmPrinter> X(getTheMyriscTarget());
}

MyriscAsmPrinter::MyriscAsmPrinter(TargetMachine &TM,
std::unique_ptr<MCStreamer> Streamer)
:AsmPrinter(TM, std::move(Streamer)) {
  STI = static_cast<MyriscTargetMachine &>(TM).getSubtargetImpl();
}
bool MyriscAsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  llvm::AsmPrinter::runOnMachineFunction(MF);

  return true;
}
void MyriscAsmPrinter::PrinterDebugValueComment(const MachineInstr *mi,
                                              raw_ostream &os) {
  // TODO: implement
  os << "PrinterDebugValueComment()";
}

#include "MyriscGenMCPseudoLowering.inc"
void MyriscAsmPrinter::emitInstruction(const MachineInstr *MI) {


  // 1. 处理调试信息（必加，避免调试信息丢失）
  if (MI->isDebugValue()) {
    SmallString<128> Str;
    raw_svector_ostream OS(Str);
    PrinterDebugValueComment(MI, OS); // LLVM封装的调试注释打印
    return;
  }

  // 2. 处理指令束（兼容LLVM指令束规范，RISC-V无束但保留框架）
  MachineBasicBlock::const_instr_iterator I = MI->getIterator();
  MachineBasicBlock::const_instr_iterator const E = MI->getParent()->instr_end();

  do {
    // 3. 自动展开伪指令（核心：展开PseudoRET/ CALL等伪指令）
    if (emitPseudoExpansionLowering(*OutStreamer, &*I)) {
      // errs() << "=== Finish pseudo instruction: " << MI->getOpcode() << "\n";
      continue; // 伪指令已展开，无需后续处理
    }
    // 4. 检查残留伪指令（防呆：伪指令未展开则崩溃，快速定位问题）
    if (I->isPseudo()) {
      llvm_unreachable("Unlowered pseudo instruction found in MyriscAsmPrinter!");
    }

    // 5. 转换MachineInstr到MCInst（使用LLVM标准的MCInstLowering）
    MCInst TmpInst;
    lowerToMCInst(&*I, TmpInst); // 替代你自定义的lowerToMCInst

    // 6. 发射指令到汇编流（LLVM 12标准写法）
    OutStreamer->emitInstruction(TmpInst, getSubtargetInfo());
    // EmitToStreamer(*OutStreamer, TmpInst);
  } while ((++I != E) && I->isInsideBundle()); // RISC-V无束，循环仅执行1次
}
bool MyriscAsmPrinter::lowerOperand(const MachineOperand &MO, MCOperand &MCOp) const{
  MCOp = LowerOperand(MO, false);
  return true;
}

/// 将MachineInstr转为MCInst，转换操作数的形式
///
void MyriscAsmPrinter::lowerToMCInst(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());
  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    MCOperand Op = LowerOperand(MO,false);
    if (Op.isValid()) {
      OutMI.addOperand(Op);
    }
  }

}
MCOperand MyriscAsmPrinter::lowerSymbolOperand(const MachineOperand &MO,
                                               bool IsBranch) const {
  return MCOperand();
}
MCOperand MyriscAsmPrinter::LowerOperand(const MachineOperand &MO,
                                         bool IsBranch) const {
  switch (MO.getType()) {
  default: llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands
    if (MO.isImplicit()) break;
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  case MachineOperand::MO_RegisterMask: {
    /// Ignore
    break;
  }
  }
  return MCOperand();
}
