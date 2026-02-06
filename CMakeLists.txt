
cmake_minimum_required(VERSION 3.31)


project(VulkanHelper)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_compile_options(
        -O0
        -march=native
        -funroll-loops
)

set(EXECUTABLE_OUTPUT_PATH bin)


add_subdirectory(src/vulkan_helper)

add_executable(VulkanHelper
        src/vkh_demo/main.cpp
        src/vkh_demo/DemoApplication.cpp
        include/vulkan_helper/window/win/Menubar.hpp
)


find_package(Vulkan REQUIRED)

target_link_libraries(VulkanHelper PRIVATE
        vulkan_helper
)

target_compile_definitions(VulkanHelper
        PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
)


target_link_directories(VulkanHelper PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(VulkanHelper PRIVATE
        # Vulkan
        Vulkan::Vulkan
)


add_custom_target(compileShaders ALL COMMAND powershell -ExecutionPolicy Bypass -File "${CMAKE_CURRENT_SOURCE_DIR}/compile.ps1")
add_dependencies(VulkanHelper compileShaders)