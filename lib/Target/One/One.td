include "llvm/Target/Target.td"

include "OneRegisterInfo.td"
include "OneInstrInfo.td"
include "OneCallingConv.td"
include "OneSchedule.td"

//===----------------------------------------------------------------------===//
// One processors supported
//===----------------------------------------------------------------------===//
//ProcessorModel<string Name, ProcessorItineraries, list<SubtargetFeature> Features>
// Name:one处理器名
// ProcessorItineraries：NoSchedModel暂未指定调度模型
// Features：[]架构暂未关联特性
def : ProcessorModel<"one", NoSchedModel, []>;

def OneInstrInfo : InstrInfo;

// “将所有伪指令的指针类型操作数重映射到指定的寄存器类”
//确保所有伪指令的 “指针操作数”（如内存地址）只能使用 GPR 寄存器，符合 One 架构的寄存器使用规则
defm : RemapAllTargetPseudoPointerOperands<GPR>;
//定义One子Target
def One : Target {
    let InstructionSet = OneInstrInfo;
    let AllowRegisterRenaming = 1;
}

