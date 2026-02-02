add_library(context OBJECT
        BufferContext.cpp
        ImageContext.cpp
        RenderContext.cpp
        SharedImageContext.cpp
        WindowContext.cpp
)
target_link_libraries(context PRIVATE vkh_dependency)