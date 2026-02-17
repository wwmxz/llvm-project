# MCTargetDesc/CMakeLists.txt
add_llvm_component_library(LLVMOneDesc
        OneMCTargetDesc.cpp
        LINK_COMPONENTS
        MC
        Support

  )

