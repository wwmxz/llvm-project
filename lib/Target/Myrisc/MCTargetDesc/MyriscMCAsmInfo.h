//
// Created by 10587 on 2026/3/6.
//

#ifndef LLVM_MYRISCMCASMINFO_H
#define LLVM_MYRISCMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class MyriscMCAsmInfo : public MCAsmInfoELF {
public:
  explicit MyriscMCAsmInfo(const Triple &TargetTriple);
};
} // namespace llvm

#endif // LLVM_MYRISCMCASMINFO_H
