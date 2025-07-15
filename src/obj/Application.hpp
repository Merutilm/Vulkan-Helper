//
// Created by Merutilm on 2025-07-07.
//

#pragma once
#include <memory>

#include "../handle/Handler.hpp"

#include "Engine.hpp"
#include "SyncObject.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "Framebuffer.hpp"
#include "RenderPass.hpp"
#include "Pipeline.hpp"
#include "VertexBuffer.hpp"


namespace merutilm::vkh {
    class Application final : public Handler{

        uint32_t currentFrame = 0;
        std::unique_ptr<Engine> engine = nullptr;
        std::unique_ptr<SyncObject> syncObject = nullptr;
        std::unique_ptr<CommandPool> commandPool = nullptr;
        std::unique_ptr<CommandBuffer> commandBuffer = nullptr;
        std::unique_ptr<RenderPass> renderPass = nullptr;
        std::unique_ptr<VertexBuffer> vertexBuffer = nullptr;
        std::unique_ptr<Framebuffer> framebuffer = nullptr;
        std::unique_ptr<DescriptorSetLayout> descriptorLayout = nullptr;
        std::unique_ptr<PipelineLayout> pipelineLayout = nullptr;
        std::unique_ptr<Pipeline> pipeline = nullptr;

    public:
        explicit Application();

        ~Application() override;

        Application(const Application &) = delete;

        Application &operator=(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(Application &&) = delete;

        void start() const;

    private:
        void init() override;

        void createEngine();

        void createRenderPass();

        void drawFrame();

        void changeFrameIndex();

        void destroy() override;
    };
}
