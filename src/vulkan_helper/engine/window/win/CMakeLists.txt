add_library(engine_window_win OBJECT
        Window.cpp
        Menubar.cpp
)

target_link_libraries(engine_window_win PRIVATE
        vkh_dependency
)