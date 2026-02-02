add_library(repo OBJECT
        GlobalDescriptorSetLayoutRepo.cpp
        GlobalPipelineLayoutRepo.cpp
        GlobalSamplerRepo.cpp
        GlobalShaderModuleRepo.cpp
        WindowLocalDescriptorRepo.cpp
)
target_link_libraries(repo PRIVATE vkh_dependency)