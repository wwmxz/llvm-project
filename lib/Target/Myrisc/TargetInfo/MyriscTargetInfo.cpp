//
// Created by 10587 on 2026/3/5.
//

#include "MyriscTargetInfo.h"

#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

llvm::Target &llvm::getTheMyriscTarget() {
  static llvm::Target TheTarget;
  return TheTarget;

}



extern "C" void LLVMInitializeMyriscTargetInfo() {
  RegisterTarget<Triple::myrisc, /*HasJIT=*/true> X(
        getTheMyriscTarget(), "myrisc", "myrisc : 32-bit", "myrisc");
}