//===-- One.h - Top-level interface for One representation ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM One back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_ONE_ONE_H
#define LLVM_LIB_TARGET_ONE_ONE_H

#include "MCTargetDesc/OneMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class OneTargetMachine;
  class FunctionPass;

} // end namespace llvm;

#endif

