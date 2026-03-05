//
// Created by 10587 on 2026/3/5.
//

#include "MyriscSubtarget.h"

#include "Myrisc.h"
#include "MyriscRegisterInfo.h"

#include "MyriscTargetMachine.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "myrisc-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "MyriscGenSubtargetInfo.inc"

llvm::MyriscSubtarget::MyriscSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                                 const MyriscTargetMachine &_TM):
MyriscGenSubtargetInfo(TT, CPU,CPU, FS),
TM(_TM),
TargetTriple(TT),
RegInfo(*this),
InstrInfo(*this),
FrameLowering(*this),
TLI(_TM,*this)
{

}
MyriscSubtarget &
MyriscSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                              const TargetMachine &TM) {
  if (CPU.empty()) {
    CPU = "myrisc";
  }
  ParseSubtargetFeatures(CPU, /*TuneCPU*/ CPU, FS);
  return *this;
}
