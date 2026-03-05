add_llvm_component_library(LLVMMyriscDesc
        MyriscMCTargetDesc.cpp

        LINK_COMPONENTS
        MC
        Support
)
