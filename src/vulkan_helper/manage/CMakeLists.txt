add_library(manage OBJECT
        DescriptorManager.cpp
        HostDataObjectManager.cpp
        PipelineLayoutManager.cpp
        PipelineManager.cpp
        RenderPassManager.cpp
)


target_link_libraries(manage PRIVATE vkh_dependency)