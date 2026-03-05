//
// Created by 10587 on 2026/3/5.
//


#ifndef LLVM_MYRISCTARGETMACHINE_H
#define LLVM_MYRISCTARGETMACHINE_H

#include "MyriscSubtarget.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class MyriscTargetMachine : public llvm::LLVMTargetMachine {
  std::unique_ptr<llvm::TargetLoweringObjectFile> TLOF;
  MyriscSubtarget STI;

public:
  MyriscTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                    CodeGenOpt::Level OL, bool JIT);
  ~MyriscTargetMachine() override;

  const MyriscSubtarget *getSubtargetImpl() const {
    return &STI;
  }

  // Can use this interface to fetch subtarget
  const MyriscSubtarget *getSubtargetImpl(const Function &F) const override{
    return &STI;
  }

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
}

#endif // LLVM_MYRISCTARGEMACHINE_H


