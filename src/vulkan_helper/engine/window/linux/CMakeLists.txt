add_library(engine_window_linux OBJECT
        Window.cpp
)

target_link_libraries(engine_window_linux PRIVATE
        vkh_dependency
)