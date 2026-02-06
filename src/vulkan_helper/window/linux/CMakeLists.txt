add_library(window_linux OBJECT
        Window.cpp
)

target_link_libraries(window_linux PRIVATE
        vkh_dependency
)