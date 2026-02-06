add_library(window OBJECT
        PlatformWindowBase.cpp
        PlatformMenubarBase.cpp
)

add_subdirectory(win)
add_subdirectory(mac)
add_subdirectory(linux)

target_link_libraries(window PRIVATE
        vkh_dependency
)