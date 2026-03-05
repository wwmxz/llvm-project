add_llvm_component_group(Myrisc)

set(LLVM_TARGET_DEFINITIONS Myrisc.td)

# Generate MyriscGenRegisterInfo.inc and MyriscGenInstrInfo.inc which included by 
#  your hand code C++ files. 
# MyriscGenRegisterInfo.inc came from MyriscRegisterInfo.td, MyriscGenInstrInfo.inc
#  came from MyriscInstrInfo.td.

tablegen(LLVM MyriscGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MyriscGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM MyriscGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM MyriscGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM MyriscGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM MyriscGenCallingConv.inc -gen-callingconv)
tablegen(LLVM MyriscGenCodeEmitter.inc -gen-emitter)
tablegen(LLVM MyriscGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM MyriscGenAsmWriter.inc -gen-asm-writer)

# MyriscCommonTableGen must be defined
add_public_tablegen_target(MyriscCommonTableGen)

# MyriscCodeGen should match with LLVMBuild.txt MyriscCodeGen
add_llvm_target(MyriscCodeGen
        MyriscTargetMachine.cpp

        LINK_COMPONENTS
        Analysis
        AsmPrinter
        CodeGen
        Core
        MC
        SelectionDAG
        Support
        MyriscDesc
        MyriscInfo
        Target
        GlobalISel
        ADD_TO_COMPONENT
        Myrisc
)

# Should match with "subdirectories = MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)

