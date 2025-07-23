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
        extern/stb_image.c
        extern/stb_image.h
        src/main.cpp
        src/core/Application.hpp
        src/core/Application.cpp
        src/handle/Handler.hpp
        src/core/Window.cpp
        src/core/Window.hpp
        src/core/ValidationLayer.cpp
        src/core/ValidationLayer.hpp
        src/exception/exception.hpp
        src/core/Instance.cpp
        src/core/Instance.hpp
        src/core/Surface.cpp
        src/core/Surface.hpp
        src/core/PhysicalDevice.cpp
        src/core/PhysicalDevice.hpp
        src/struct/QueueFamilyIndices.hpp
        src/core/LogicalDevice.cpp
        src/core/LogicalDevice.hpp
        src/struct/StringHasher.hpp
        src/core/Swapchain.cpp
        src/core/Swapchain.hpp
        src/core/CommandPool.cpp
        src/core/CommandPool.hpp
        src/core/CommandBuffer.cpp
        src/core/CommandBuffer.hpp
        src/core/Descriptor.cpp
        src/core/Descriptor.hpp
        src/manage/ShaderObjectManager.hpp
        src/core/BufferObject.cpp
        src/core/BufferObject.hpp
        src/util/BufferUtils.hpp
        src/manage/DescriptorManager.hpp
        src/core/Pipeline.cpp
        src/core/Pipeline.hpp
        src/core/RenderPass.cpp
        src/core/RenderPass.hpp
        src/manage/PipelineManager.hpp
        src/core/DescriptorSetLayout.hpp
        src/struct/DescriptorSetLayoutBuildType.hpp
        src/core/PipelineLayout.hpp
        src/core/DescriptorSetLayout.cpp
        src/core/PipelineLayout.cpp
        src/manage/PushConstantManager.hpp
        src/core/ShaderModule.hpp
        src/core/ShaderModule.cpp
        src/manage/RenderPassManager.hpp
        src/struct/RenderPassAttachmentType.hpp
        src/core/Core.cpp
        src/core/Core.hpp
        src/handle/CoreHandler.hpp
        src/struct/WindowCreateInfo.hpp
        src/core/Framebuffer.cpp
        src/core/Framebuffer.hpp
        src/struct/RenderPassAttachmentConfig.hpp
        src/struct/ImageInitInfo.hpp
        src/core/SyncObject.cpp
        src/core/SyncObject.hpp
        src/executor/RenderPassExecutor.cpp
        src/executor/RenderPassExecutor.hpp
        src/struct/Vertex.hpp
        src/core/VertexBuffer.cpp
        src/core/VertexBuffer.hpp
        src/manage/ConstShaderObjectManager.hpp
        src/core/UniformBuffer.cpp
        src/core/UniformBuffer.hpp
        src/executor/Executor.hpp
        src/repo/ShaderModuleRepo.hpp
        src/manage/DescriptorSetLayoutManager.hpp
        src/context/WindowContext.hpp
        src/context/RenderContext.hpp
        src/hash/VectorHasher.hpp
        src/hash/PointerDerefEquals.hpp
        src/repo/Repo.hpp
        src/repo/Repositories.hpp
        src/repo/IRepo.hpp
        src/manage/PipelineLayoutManager.hpp
        src/hash/BoostHasher.hpp
        src/repo/PipelineLayoutRepo.hpp
        src/core/IndexBuffer.cpp
        src/core/IndexBuffer.hpp
        src/core/Texture.cpp
        src/core/Texture.hpp
        src/configurator/VFPipelineConfigurator.cpp
        src/configurator/VFPipelineConfigurator.hpp
        src/repo/DescriptorRepo.hpp
        src/struct/DescriptorName.hpp
        src/template/DSLTemplate.hpp
        src/template/DSTemplate.hpp
        src/core/Engine.cpp
        src/core/Engine.hpp
        src/configurator/TemplatePipelineConfigurator.cpp
        src/configurator/TemplatePipelineConfigurator.hpp
        src/executor/ScopedCommandExecutor.cpp
        src/executor/ScopedCommandExecutor.hpp
        src/struct/PushConstantReserve.hpp
        src/configurator/RenderPassConfigurator.hpp
        src/configurator/BasicRenderPassConfigurator.hpp
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