//
// Created by 10587 on 2026/3/5.
//

#ifndef LLVM_MYRISCSELLOWERING_H
#define LLVM_MYRISCSELLOWERING_H

#include "Myrisc.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/IR/Function.h"


namespace llvm{
class  MyriscSubtarget;
namespace MyriscISD {
enum NodeType: unsigned{
  FIRST_NUMBER=ISD::BUILTIN_OP_END,
  RET_GLUE,
  CALL,
  HI,
  LO

};
}

class MyriscTargetLowering: public TargetLowering {
  const MyriscSubtarget &STI;
public:
  explicit MyriscTargetLowering(const TargetMachine &TM, const MyriscSubtarget &STI);
  const MyriscSubtarget &getSubtarget () const { return STI; }
  SDValue LowerCall(CallLoweringInfo &CLI, SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  const char *getTargetNodeName(unsigned Opcode) const override;
private:
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerConstant(SDValue Op, SelectionDAG &DAG) const;
};


}

#endif // LLVM_MYRISCSELLOWERING_H
