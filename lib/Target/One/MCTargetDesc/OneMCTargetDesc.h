//===-- OneMCTargetDesc.h - One Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides One specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_ONE_MCTARGETDESC_ONEMCTARGETDESC_H
#define LLVM_LIB_TARGET_ONE_MCTARGETDESC_ONEMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class Target;
class Triple;

extern Target TheOneTarget;

} // End llvm namespace

// Defines symbolic names for One registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "OneGenRegisterInfo.inc"

// Defines symbolic names for the One instructions.
#define GET_INSTRINFO_ENUM
#include "OneGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "OneGenSubtargetInfo.inc"

#endif

