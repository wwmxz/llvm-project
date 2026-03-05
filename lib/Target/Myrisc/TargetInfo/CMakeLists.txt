add_llvm_component_library(LLVMMyriscInfo
        MyriscTargetInfo.cpp

        LINK_COMPONENTS
        MC
        Support
)