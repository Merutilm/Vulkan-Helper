
add_subdirectory(dependency)
add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(util)


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
        $<TARGET_OBJECTS:engine_window>
)

if(WIN32)
    target_sources(vulkan_helper PUBLIC $<TARGET_OBJECTS:engine_window_win>)
elseif (APPLE)
    target_sources(vulkan_helper PUBLIC $<TARGET_OBJECTS:engine_window_mac>)
elseif (UNIX)
    target_sources(vulkan_helper PUBLIC $<TARGET_OBJECTS:engine_window_linux>)
else()
    message(STATUS "failed to find OS")
endif ()



target_link_libraries(vulkan_helper PUBLIC
        vkh_dependency

)