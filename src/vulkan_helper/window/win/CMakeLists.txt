add_library(window_win OBJECT
        Window.cpp
        Menubar.cpp
)

target_link_libraries(window_win PRIVATE
        vkh_dependency
)