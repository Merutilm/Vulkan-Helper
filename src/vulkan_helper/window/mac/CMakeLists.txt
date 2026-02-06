add_library(window_mac OBJECT
        Window.cpp
)

target_link_libraries(window_mac PRIVATE
        vkh_dependency
)