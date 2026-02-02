add_library(configurator OBJECT
        ComputePipelineConfigurator.cpp
        GeneralGraphicsPipelineConfigurator.cpp
        GeneralPostProcessGraphicsPipelineConfigurator.cpp
)

target_link_libraries(configurator PRIVATE vkh_dependency)