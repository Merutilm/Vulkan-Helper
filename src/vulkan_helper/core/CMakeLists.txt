add_library(core OBJECT
        BufferObject.cpp
        CombinedImageSampler.cpp
        CommandBuffer.cpp
        CommandPool.cpp
        ComputeShaderPipeline.cpp
        Core.cpp
        Descriptor.cpp
        DescriptorSetLayout.cpp
        Engine.cpp
        Fence.cpp
        Framebuffer.cpp
        GraphicsContextWindow.cpp
        GraphicsPipeline.cpp
        IndexBuffer.cpp
        Instance.cpp
        LogicalDevice.cpp
        PhysicalDeviceLoader.cpp
        Pipeline.cpp
        PipelineLayout.cpp
        PushConstant.cpp
        Renderer.cpp
        RenderPass.cpp
        Sampler.cpp
        Semaphore.cpp
        ShaderModule.cpp
        ShaderStorage.cpp
        Surface.cpp
        Swapchain.cpp
        SyncObject.cpp
        Uniform.cpp
        ValidationLayer.cpp
        VertexBuffer.cpp
)

target_link_libraries(core PRIVATE vkh_dependency)