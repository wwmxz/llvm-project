//===-- OneTargetInfo.cpp - One Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "One.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheOneTarget;

extern "C" void LLVMInitializeOneTargetInfo() {
  // 将Triple::one对应TheOneTarget注册给全局目标注册表
  //
  RegisterTarget<Triple::one,
        /*HasJIT=*/true> Y(TheOneTarget, "One", "One (32-bit little endian)", "One");
}

