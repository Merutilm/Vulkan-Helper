add_library(engine STATIC
        Engine.cpp
)

add_subdirectory(buffer)
add_subdirectory(cmd)
add_subdirectory(configurator)
add_subdirectory(context)
add_subdirectory(descriptor)
add_subdirectory(executor)
add_subdirectory(graphics)
add_subdirectory(manage)
add_subdirectory(pipeline)
add_subdirectory(repo)
add_subdirectory(sampler)
add_subdirectory(sync)

target_link_libraries(engine PRIVATE
        vkh_dependency
)

target_link_libraries(engine PUBLIC
        engine_buffer
        engine_cmd
        engine_configurator
        engine_context
        engine_descriptor
        engine_executor
        engine_graphics
        engine_manage
        engine_pipeline
        engine_repo
        engine_sampler
        engine_sync
)