add_llvm_component_library(LLVMMyriscDesc
        MyriscMCTargetDesc.cpp
        MyriscInstPrinter.cpp
        MyriscMCAsmInfo.cpp

        LINK_COMPONENTS
        MC
        Support
)
