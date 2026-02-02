add_library(executor OBJECT
        RenderPassFullscreenRecorder.cpp
        ScopedCommandBufferExecutor.cpp
        ScopedNewCommandBufferExecutor.cpp
)

target_link_libraries(executor PRIVATE vkh_dependency)