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

#ifndef LLVM_LIB_TARGET_MYRISC_H
#define LLVM_LIB_TARGET_MYRISC_H

#include "MCTargetDesc/MyriscMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
#define DIV_ROUND_UP(n, d)  (((n) + (d) - 1) / (d))
#define ROUND_UP(x, align)  (DIV_ROUND_UP(x, align) * (align))
class MyriscTargetMachine;
class FunctionPass;
FunctionPass *createMyriscISelDag(MyriscTargetMachine &TM,CodeGenOpt::Level OptLevel);
} // end namespace llvm;

#endif

