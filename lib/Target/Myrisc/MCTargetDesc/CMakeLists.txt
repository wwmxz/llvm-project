add_llvm_component_library(LLVMMyriscDesc
        MyriscMCTargetDesc.cpp
        MyriscInstPrinter.cpp
        MyriscMCAsmInfo.cpp
        MyriscMCExpr.cpp

        LINK_COMPONENTS
        MC
        Support
)
