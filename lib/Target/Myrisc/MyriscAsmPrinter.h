//
// Created by 10587 on 2026/3/6.
//

#ifndef LLVM_MYRISCASMPRINTER_H
#define LLVM_MYRISCASMPRINTER_H


#include "MyriscTargetMachine.h"
// #include "MyriscMCInstLower.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Target/TargetMachine.h"



namespace llvm {
class MyriscSubtarget;

class LLVM_LIBRARY_VISIBILITY MyriscAsmPrinter:public AsmPrinter {
  const MyriscSubtarget* STI;
public:
  explicit MyriscAsmPrinter(TargetMachine& TM,std::unique_ptr<MCStreamer> Streamer);
  virtual bool runOnMachineFunction(MachineFunction &MF) override;
  void PrinterDebugValueComment(const MachineInstr * mi, raw_ostream & os);
  void emitInstruction(const MachineInstr *MI) override;
  virtual StringRef getPassName() const override {
    return "Myrisc Assembly Printer";
  }
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCO) const;
  /// "MyriscGenMCPseudoLowering.inc"“— µœ÷
  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,const MachineInstr *MI);
private:

  void lowerToMCInst(const MachineInstr *MI, MCInst &Out) const;
  MCOperand lowerSymbolOperand(const MachineOperand &MO, bool IsBranch) const;
  MCOperand LowerOperand(const MachineOperand &MO, bool IsBranch) const;
};


}



#endif // LLVM_MYRISCASMPRINTER_H
