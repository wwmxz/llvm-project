//
// Created by 10587 on 2026/3/5.
//

#include "MyriscTargetMachine.h"

#include "Myrisc.h"
#include "TargetInfo/MyriscTargetInfo.h"

#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetOptions.h"


using namespace llvm;

#define DEBUG_TYPE "myrisc"

extern "C" void LLVMInitializeMyriscTarget() {
  RegisterTargetMachine<MyriscTargetMachine> X(getTheMyriscTarget());
}

// 定义Myrisc架构的内存布局、重定位模型和代码模型等属性

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  outs()<<"TT.March: "<<TT.getArchName()<<"\n";
  assert(TT.isMyrisc()&& "only Myrisc is currently supported");
  assert(TT.isArch32Bit() && "only 32 bit are currently supported");
  std::string Ret = "";
  Ret += "e";

  Ret += "-m:e";

  // Pointer size and alignment
  Ret += "-p:32:32";

  // 8 and 16 bits integers only need to have natural alignment, but try to
  // align them to 32 bits. 64 bits integers have natural alignment.
  Ret += "-i64:64";

  // 32 bits registers are always available and the stack is at least 64 bits aligned
  Ret += "-n32-S128";

  return Ret;
}

static Reloc::Model getEffectiveRelocModel(bool JIT,
                                           Optional<Reloc::Model> RM) {
  if (!RM.hasValue() || JIT)
    return Reloc::Static;
  return *RM;
}

MyriscTargetMachine::MyriscTargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     Optional<CodeModel::Model> CM,
                                     CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT,
                        CPU, FS, Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      STI(TT, CPU, FS, *this)
{
  initAsmInfo();
}

MyriscTargetMachine::~MyriscTargetMachine() { }

namespace {

class MyriscPassConfig : public TargetPassConfig {
public:
  MyriscPassConfig(MyriscTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  MyriscTargetMachine& getMyriscTargetMachine() const {
    return getTM<MyriscTargetMachine>();
  }

  const MyriscSubtarget& getMyriscSubtarget() const {
    return *getTM<MyriscTargetMachine>().getSubtargetImpl();
  }
  // bool addInstSelector() override;

};

}//namespace

TargetPassConfig *MyriscTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new MyriscPassConfig(*this, PM);
}
