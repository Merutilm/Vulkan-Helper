add_library(core OBJECT
        Core.cpp
        GraphicsContextWindow.cpp
        Instance.cpp
        LogicalDevice.cpp
        PhysicalDeviceLoader.cpp
        Surface.cpp
        Swapchain.cpp
        ValidationLayer.cpp
)

target_link_libraries(core PRIVATE vkh_dependency)