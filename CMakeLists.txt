
cmake_minimum_required(VERSION 3.31)


project(VulkanHelper)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib_export)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib_export)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src/vulkan_helper)

add_executable(VulkanHelper
        src/vkh_demo/main.cpp
        src/vkh_demo/DemoApplication.cpp
        src/vkh_demo/SampleRCC.cpp
        src/vkh_demo/SampleRCC.hpp
        src/vkh_demo/GPCSample.cpp
        src/vkh_demo/GPCSample.hpp
        src/vkh_demo/SampleRenderer.cpp
        src/vkh_demo/SampleRenderer.hpp
)

target_compile_options(VulkanHelper PRIVATE
        #Debug options
        $<$<CONFIG:Debug>:-O0>
        #Release options
        $<$<CONFIG:Release>:-Ofast -funroll-loops -march=native>
)

find_package(Vulkan REQUIRED)

target_link_libraries(VulkanHelper PRIVATE
        vulkan_helper
)

target_compile_definitions(VulkanHelper
        PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
)

add_custom_target(compileShaders ALL COMMAND powershell -ExecutionPolicy Bypass -File "${CMAKE_CURRENT_SOURCE_DIR}/compile.ps1")
add_dependencies(VulkanHelper compileShaders)