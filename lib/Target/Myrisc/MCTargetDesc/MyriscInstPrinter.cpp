//
// Created by 10587 on 2026/3/6.
//


#include "MyriscInstPrinter.h"
#include "MyriscInstrInfo.h"
#include "MyriscRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "MyriscGenAsmWriter.inc"

void MyriscInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) {
  OS << getRegisterName(Reg);
}

void MyriscInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                               StringRef Annot, const MCSubtargetInfo &STI,
                               raw_ostream &O) {
  if (!printAliasInstr(MI, Address, O))
    printInstruction(MI, Address, O);

  printAnnotation(O, Annot);
}

void MyriscInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(OpNo);
  if (MO.isReg()) {
    printRegName(O, MO.getReg());
    return;
  }

  if (MO.isImm()) {
    printImmediate(MI, OpNo, O);
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI);
}

void MyriscInstPrinter::printImmediate(const MCInst *MI, unsigned opNum,
                                    raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(opNum);
  if (MO.isImm())
    O << MO.getImm();
  else if (MO.isExpr()) {
    MO.getExpr()->print(O, &MAI);
  } else
    llvm_unreachable("Unknown immediate kind");
}

void MyriscInstPrinter::printMemOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O) {
  /// 先打印立即数 12(sp)
  printOperand(MI, OpNo + 1, O);
  O << "(";
  printOperand(MI, OpNo, O);
  O << ")";
}

void MyriscInstPrinter::printPtrOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O) {
  printOperand(MI, OpNo, O);
  O << ",,,";
  printOperand(MI, OpNo+1, O);
}