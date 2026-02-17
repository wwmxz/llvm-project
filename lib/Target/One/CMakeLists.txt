add_llvm_component_group(One)

set(LLVM_TARGET_DEFINITIONS One.td)

# Generate OneGenRegisterInfo.inc and OneGenInstrInfo.inc which included by 
#  your hand code C++ files. 
# OneGenRegisterInfo.inc came from OneRegisterInfo.td, OneGenInstrInfo.inc
#  came from OneInstrInfo.td.
tablegen(LLVM OneGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM OneGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM OneGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM OneGenMCPseudoLowering.inc -gen-pseudo-lowering)

# OneCommonTableGen must be defined
add_public_tablegen_target(OneCommonTableGen)

# OneCodeGen should match with LLVMBuild.txt OneCodeGen
add_llvm_target(OneCodeGen
        OneTargetMachine.cpp
        LINK_COMPONENTS
        Analysis
        AsmPrinter
        CodeGen
        Core
        MC
        SelectionDAG
        Support
        OneDesc
        OneInfo
        Target
        GlobalISel
        ADD_TO_COMPONENT
        One
  )

# Should match with "subdirectories = MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)

