add_library(vulkan_helper STATIC
        Application.cpp
        $<TARGET_OBJECTS:core>
        $<TARGET_OBJECTS:util>
        $<TARGET_OBJECTS:engine>
        $<TARGET_OBJECTS:engine_buffer>
        $<TARGET_OBJECTS:engine_cmd>
        $<TARGET_OBJECTS:engine_configurator>
        $<TARGET_OBJECTS:engine_context>
        $<TARGET_OBJECTS:engine_descriptor>
        $<TARGET_OBJECTS:engine_executor>
        $<TARGET_OBJECTS:engine_graphics>
        $<TARGET_OBJECTS:engine_manage>
        $<TARGET_OBJECTS:engine_pipeline>
        $<TARGET_OBJECTS:engine_repo>
        $<TARGET_OBJECTS:engine_sampler>
        $<TARGET_OBJECTS:engine_sync>
)

add_subdirectory(dependency)
add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(util)

target_link_libraries(vulkan_helper PUBLIC
        vkh_dependency

)