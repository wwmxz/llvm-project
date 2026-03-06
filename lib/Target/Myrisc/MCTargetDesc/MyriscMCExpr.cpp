//
// Created by 10587 on 2026/3/6.
//

#include "MyriscMCExpr.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

void MyriscMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  switch (Kd) {
  case HI:
    OS << "%hi(";
    break;
  case LO:
    OS << "%lo(";
    break;
  default:
    break;
  }
  Expr->print(OS, MAI, true);

  switch (Kd) {
  case HI:
  case LO:
    OS << ")";
    break;
  default:
    break;
  }
}
