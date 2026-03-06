//
// Created by 10587 on 2026/3/6.
//

#ifndef LLVM_MYRISCMCEXPR_H
#define LLVM_MYRISCMCEXPR_H

#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCStreamer.h"

namespace llvm {
class MyriscMCExpr : public MCTargetExpr {
public:
  enum Kind { NONE, HI, LO, JAL, Branch };
  MyriscMCExpr(Kind K, const MCExpr *Expr) : Kd(K), Expr(Expr) {}

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override {
    return Expr->evaluateAsRelocatable(Res, Layout, Fixup);
  };
  void visitUsedExpr(MCStreamer &Streamer) const override {
    // NOTE: MyriscMCExpr 的 kind 是 Target, 导致 MCStreamer 不知道如何 visit 它的
    // operand, 需要自己实现. visitUsedExpr 会把 operand 中用到的 symbol ref 加
    // 入到符号表中
    Streamer.visitUsedExpr(*Expr);
  }
  MCFragment *findAssociatedFragment() const override { return nullptr; }

  void fixELFSymbolsInTLSFixups(MCAssembler &) const override {}
  Kind getKind() const { return Kd; }
private:
  const Kind Kd;
  const MCExpr *Expr;
};
} // namespace llvm


#endif // LLVM_MYRISCMCEXPR_H
