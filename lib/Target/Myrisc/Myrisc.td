include "llvm/Target/Target.td"

// Register Infomation
include "MyriscRegisterInfo.td"
// Instruction Description
include "MyriscInstrInfo.td"
// Calling Conversion
include "MyriscSchedule.td"
// Calling Convention
include "MyriscCallingConv.td"

//===----------------------------------------------------------------------===//
// Myrisc processors supported
//===----------------------------------------------------------------------===//
//ProcessorModel<string Name, ProcessorItineraries, list<SubtargetFeature> Features>
// Name:myrisc处理器名
// ProcessorItineraries：NoSchedModel暂未指定调度模型
// Features：[]架构暂未关联特性
def : ProcessorModel<"myrisc", NoSchedModel, []>;

def MyriscInstrInfo : InstrInfo;

// “将所有伪指令的指针类型操作数重映射到指定的寄存器类”
//确保所有伪指令的 “指针操作数”（如内存地址）只能使用 GPR 寄存器，符合 Myrisc 架构的寄存器使用规则
//defm : RemapAllTargetPseudoPointerOperands<GPR>;
//定义Myrisc子Target
def Myrisc : Target {
    let InstructionSet = MyriscInstrInfo;
    let AllowRegisterRenaming = 1;
}

