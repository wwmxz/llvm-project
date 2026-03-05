//
// Created by 10587 on 2026/3/5.
//

#ifndef LLVM_MYRISCMCTARGETDESC_H
#define LLVM_MYRISCMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class Target;
class Triple;

Target &getTheMyriscTarget();

} // End llvm namespace

// Defines symbolic names for Myrisc registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "MyriscGenRegisterInfo.inc"

// Defines symbolic names for the Myrisc instructions.
#define GET_INSTRINFO_ENUM
#include "MyriscGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "MyriscGenSubtargetInfo.inc"

#endif // LLVM_MYRISCMCTARGETDESC_H
