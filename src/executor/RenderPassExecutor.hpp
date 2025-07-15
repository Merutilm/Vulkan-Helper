//
// Created by Merutilm on 2025-07-15.
//

#pragma once
#include "../obj/CommandBuffer.hpp"
#include "../obj/Framebuffer.hpp"
#include "../obj/RenderPass.hpp"
#include "../handle/EngineHandler.hpp"

namespace merutilm::vkh {
    class RenderPassExecutor final : public EngineHandler {
        const CommandBuffer &commandBuffer;
        const RenderPass &renderPass;
        const Framebuffer &framebuffer;
        const uint32_t frameIndex;
        const uint32_t imageIndex;

    public:
        explicit RenderPassExecutor(const Engine &engine, const CommandBuffer &commandBuffer,
                                    const RenderPass &renderPass,
                                    const Framebuffer &framebuffer, uint32_t frameIndex, uint32_t imageIndex);

        ~RenderPassExecutor() override;

        RenderPassExecutor(const RenderPassExecutor &) = delete;

        RenderPassExecutor &operator=(const RenderPassExecutor &) = delete;

        RenderPassExecutor(RenderPassExecutor &&) = delete;

        RenderPassExecutor &operator=(RenderPassExecutor &&) = delete;

    private:
        void init() override;

        void destroy() override;
    };
}
