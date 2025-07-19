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
        src/struct/DescriptorSetLayoutBuildType.hpp
        src/obj/PipelineLayout.hpp
        src/obj/DescriptorSetLayout.cpp
        src/obj/PipelineLayout.cpp
        src/manage/PushConstantManager.hpp
        src/obj/ShaderModule.hpp
        src/obj/ShaderModule.cpp
        src/manage/RenderPassManager.hpp
        src/struct/RenderPassAttachmentType.hpp
        src/base/Core.cpp
        src/base/Core.hpp
        src/handle/CoreHandler.hpp
        src/struct/WindowCreateInfo.hpp
        src/obj/Framebuffer.cpp
        src/obj/Framebuffer.hpp
        src/struct/RenderPassAttachmentConfig.hpp
        src/struct/ImageInitInfo.hpp
        src/obj/SyncObject.cpp
        src/obj/SyncObject.hpp
        src/executor/RenderPassExecutor.cpp
        src/executor/RenderPassExecutor.hpp
        src/struct/Vertex.hpp
        src/obj/VertexBuffer.cpp
        src/obj/VertexBuffer.hpp
        src/manage/ConstBufferObjectManager.hpp
        src/obj/UniformBuffer.cpp
        src/obj/UniformBuffer.hpp
        src/executor/Executor.hpp
        src/repo/ShaderModuleRepo.hpp
        src/manage/DescriptorSetLayoutManager.hpp
        src/context/WindowContext.hpp
        src/context/RenderContext.hpp
        src/hash/VectorHasher.hpp
        src/hash/PointerDerefEquals.hpp
        src/hash/DescriptorSetLayoutBuildTypeHasher.hpp
        src/repo/Repo.hpp
        src/repo/Repositories.hpp
        src/repo/IRepo.hpp
        src/manage/PipelineLayoutManager.hpp
        src/hash/BoostHasher.hpp
        src/repo/PipelineLayoutRepo.hpp
        src/obj/IndexBuffer.cpp
        src/obj/IndexBuffer.hpp
        src/obj/Texture.cpp
        src/obj/Texture.hpp
        src/base/BasicShaderProgram.cpp
        src/base/BasicShaderProgram.hpp
        src/repo/DescriptorRepo.hpp
        src/struct/DescriptorName.hpp
        src/template/DSLTemplate.hpp
        src/template/DSTemplate.hpp
        src/base/Engine.cpp
        src/base/Engine.hpp
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