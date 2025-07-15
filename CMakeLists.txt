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

add_executable(VulkanHelper
        src/main.cpp
        src/obj/Application.hpp
        src/obj/Application.cpp
        src/handle/Handler.hpp
        src/obj/Window.cpp
        src/obj/Window.hpp
        src/obj/ValidationLayer.cpp
        src/obj/ValidationLayer.hpp
        src/exception/exception.hpp
        src/obj/Instance.cpp
        src/obj/Instance.hpp
        src/obj/Surface.cpp
        src/obj/Surface.hpp
        src/obj/PhysicalDevice.cpp
        src/obj/PhysicalDevice.hpp
        src/struct/QueueFamilyIndices.hpp
        src/obj/LogicalDevice.cpp
        src/obj/LogicalDevice.hpp
        src/struct/StringHasher.hpp
        src/obj/Swapchain.cpp
        src/obj/Swapchain.hpp
        src/obj/CommandPool.cpp
        src/obj/CommandPool.hpp
        src/obj/CommandBuffer.cpp
        src/obj/CommandBuffer.hpp
        src/obj/Descriptor.cpp
        src/obj/Descriptor.hpp
        src/manage/BufferObjectManager.hpp
        src/obj/BufferObject.cpp
        src/obj/BufferObject.hpp
        src/util/BufferUtils.hpp
        src/manage/DescriptorManager.hpp
        src/obj/Pipeline.cpp
        src/obj/Pipeline.hpp
        src/obj/RenderPass.cpp
        src/obj/RenderPass.hpp
        src/manage/PipelineManager.hpp
        src/obj/DescriptorSetLayout.hpp
        src/struct/DescriptorLayoutBuildType.hpp
        src/obj/PipelineLayout.hpp
        src/obj/DescriptorSetLayout.cpp
        src/obj/PipelineLayout.cpp
        src/manage/PushConstant.hpp
        src/obj/ShaderModule.hpp
        src/obj/ShaderModule.cpp
        src/manage/RenderPassManager.hpp
        src/struct/RenderPassAttachmentType.hpp
        src/obj/Engine.cpp
        src/obj/Engine.hpp
        src/handle/EngineHandler.hpp
        src/struct/EngineInitInfo.hpp
        src/obj/Framebuffer.cpp
        src/obj/Framebuffer.hpp
        src/struct/RenderPassAttachment.hpp
        src/struct/ImageInitInfo.hpp
        src/obj/SyncObject.cpp
        src/obj/SyncObject.hpp
        src/executor/CommandExecutor.cpp
        src/executor/CommandExecutor.hpp
        src/executor/RenderPassExecutor.cpp
        src/executor/RenderPassExecutor.hpp
        src/struct/Vertex.hpp
        src/obj/VertexBuffer.cpp
        src/obj/VertexBuffer.hpp
        src/manage/Uploader.hpp
        src/obj/UniformBuffer.cpp
        src/obj/UniformBuffer.hpp
        src/executor/Executor.hpp
)


find_package(Vulkan REQUIRED)

target_compile_definitions(VulkanHelper
        PRIVATE
        VK_USE_PLATFORM_WIN32_KHR
)

target_include_directories(VulkanHelper PRIVATE
        # standard include
        ${CMAKE_SOURCE_DIR}/include
        # extern sources
        ${CMAKE_SOURCE_DIR}/extern
        # vulkan sdk directory
        ${Vulkan_INCLUDE_DIRS}
)


target_link_directories(VulkanHelper PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(VulkanHelper PRIVATE
        # Vulkan
        Vulkan::Vulkan
)


add_custom_target(compileShaders ALL COMMAND powershell -ExecutionPolicy Bypass -File "${CMAKE_CURRENT_SOURCE_DIR}/compile.ps1")
add_dependencies(VulkanHelper compileShaders)