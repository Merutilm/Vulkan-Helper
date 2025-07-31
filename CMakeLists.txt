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
        src/def/Application.hpp
        src/def/Application.cpp
        src/handle/Handler.hpp
        src/def/Window.cpp
        src/def/Window.hpp
        src/def/ValidationLayer.cpp
        src/def/ValidationLayer.hpp
        src/exception/exception.hpp
        src/def/Instance.cpp
        src/def/Instance.hpp
        src/def/Surface.cpp
        src/def/Surface.hpp
        src/def/PhysicalDevice.cpp
        src/def/PhysicalDevice.hpp
        src/struct/QueueFamilyIndices.hpp
        src/def/LogicalDevice.cpp
        src/def/LogicalDevice.hpp
        src/struct/StringHasher.hpp
        src/def/Swapchain.cpp
        src/def/Swapchain.hpp
        src/def/CommandPool.cpp
        src/def/CommandPool.hpp
        src/def/CommandBuffer.cpp
        src/def/CommandBuffer.hpp
        src/def/Descriptor.cpp
        src/def/Descriptor.hpp
        src/manage/ShaderObjectManager.hpp
        src/def/BufferObject.cpp
        src/def/BufferObject.hpp
        src/util/BufferImageUtils.hpp
        src/manage/DescriptorManager.hpp
        src/def/Pipeline.cpp
        src/def/Pipeline.hpp
        src/def/RenderPass.cpp
        src/def/RenderPass.hpp
        src/manage/PipelineManager.hpp
        src/def/DescriptorSetLayout.hpp
        src/struct/DescriptorSetLayoutBuildType.hpp
        src/def/PipelineLayout.hpp
        src/def/DescriptorSetLayout.cpp
        src/def/PipelineLayout.cpp
        src/manage/PushConstantManager.hpp
        src/def/ShaderModule.hpp
        src/def/ShaderModule.cpp
        src/manage/RenderPassManager.hpp
        src/struct/RenderPassAttachmentType.hpp
        src/def/Core.cpp
        src/def/Core.hpp
        src/handle/CoreHandler.hpp
        src/struct/WindowCreateInfo.hpp
        src/def/Framebuffer.cpp
        src/def/Framebuffer.hpp
        src/struct/RenderPassAttachment.hpp
        src/struct/ImageInitInfo.hpp
        src/def/SyncObject.cpp
        src/def/SyncObject.hpp
        src/executor/RenderPassExecutor.cpp
        src/executor/RenderPassExecutor.hpp
        src/struct/Vertex.hpp
        src/def/VertexBuffer.cpp
        src/def/VertexBuffer.hpp
        src/manage/CompleteShaderObjectManager.hpp
        src/def/Uniform.cpp
        src/def/Uniform.hpp
        src/executor/Executor.hpp
        src/repo/ShaderModuleRepo.hpp
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
        src/def/IndexBuffer.cpp
        src/def/IndexBuffer.hpp
        src/def/Sampler2D.cpp
        src/def/Sampler2D.hpp
        src/configurator/GeneralPipelineConfigurator.cpp
        src/configurator/GeneralPipelineConfigurator.hpp
        src/repo/DescriptorRepo.hpp
        src/struct/DescriptorName.hpp
        src/template/DSTemplate.hpp
        src/def/Engine.cpp
        src/def/Engine.hpp
        src/configurator/Template2PipelineConfigurator.cpp
        src/configurator/Template2PipelineConfigurator.hpp
        src/executor/ScopedCommandExecutor.cpp
        src/executor/ScopedCommandExecutor.hpp
        src/struct/PushConstantReserve.hpp
        src/configurator/RenderContextConfigurator.hpp
        src/configurator/BasicRenderContextConfigurator.hpp
        src/hash/DescriptorSetLayoutBuildTypeHasher.hpp
        src/context/ImageContext.hpp
        src/util/Presenter.hpp
        src/configurator/Template1PipelineConfigurator.cpp
        src/configurator/Template1PipelineConfigurator.hpp
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