//
// Created by 10587 on 2026/3/5.
//

#include "MyriscMCTargetDesc.h"

#include "llvm/Support/Compiler.h"

#include "MyriscMCTargetDesc.h"
// #include "MyriscAsmBackend.h"
// #include "MyriscMCCodeEmitter.h"
#include "MyriscInstPrinter.h"
#include "MyriscInstrInfo.h"
#include "MyriscMCAsmInfo.h"
#include "MyriscRegisterInfo.h"
#include "MyriscSubtarget.h"
#include "TargetInfo/MyriscTargetInfo.h"

#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "MyriscGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "MyriscGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "MyriscGenRegisterInfo.inc"

MCRegisterInfo *createMyriscMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitMyriscMCRegisterInfo(X, Myrisc::RA);
  return X;
}

MCInstrInfo *createMyriscMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitMyriscMCInstrInfo(X);
  return X;
}

MCSubtargetInfo *createMyriscMCSubtargetInfo(const Triple &TT, StringRef CPU,
                                          StringRef FS) {
  if (CPU.empty()) {
    CPU = "myrisc";
  }
  return createMyriscMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}

MCInstPrinter *createMyriscMCInstPrinter(const Triple &T, unsigned SyntaxVariant,
                                      const MCAsmInfo &MAI,
                                      const MCInstrInfo &MII,
                                      const MCRegisterInfo &MRI) {
  return new MyriscInstPrinter(MAI, MII, MRI);
}

MCAsmInfo *createMyriscMCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT,
                              const MCTargetOptions &Options) {
  return new MyriscMCAsmInfo(TT);
}


extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMyriscTargetMC() {
  TargetRegistry::RegisterMCRegInfo(getTheMyriscTarget(), createMyriscMCRegisterInfo);
  TargetRegistry::RegisterMCInstrInfo(getTheMyriscTarget(), createMyriscMCInstrInfo);
  TargetRegistry::RegisterMCSubtargetInfo(getTheMyriscTarget(),
                                          createMyriscMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(getTheMyriscTarget(),
                                      createMyriscMCInstPrinter);
  TargetRegistry::RegisterMCAsmInfo(getTheMyriscTarget(), createMyriscMCAsmInfo);
}