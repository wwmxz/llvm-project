//
// Created by 10587 on 2026/3/6.
//

#ifndef LLVM_MYRISCSELDAGTODAG_H
#define LLVM_MYRISCSELDAGTODAG_H

#include "MCTargetDesc/MyriscMCTargetDesc.h"
#include "Myrisc.h"
#include "MyriscSubtarget.h"
#include "MyriscTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"

namespace llvm{
class MyriscDAGToDAGISel : public SelectionDAGISel {
public:
  MyriscDAGToDAGISel() = delete;
  explicit MyriscDAGToDAGISel(MyriscTargetMachine &TM, CodeGenOpt::Level OL)
      : SelectionDAGISel(TM, OL), Subtarget(nullptr) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  bool SelectAddrFI(SDNode *Parent, SDValue AddrFI, SDValue &BASE, SDValue &Offset);
  StringRef getPassName() const override {
    return "Myrisc DAG to DAG Pattern Instruction Selection";
  }
private:
  const MyriscSubtarget *Subtarget;

#include "MyriscGenDAGISel.inc"

  /// getTargetMachine - Return a reference to the TargetMachine, casted
  /// to the target-specific type.
  const MyriscTargetMachine &getTargetMachine() {
    return static_cast<const MyriscTargetMachine &>(TM);
  }
public:
  void Select(SDNode *N) override;
};

}

#endif // LLVM_MYRISCSELDAGTODAG_H
