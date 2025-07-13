//
// Created by Merutilm on 2025-07-09.
//

#pragma once
#include "CommandPool.hpp"
#include "../objhandler/EngineHandler.hpp"

namespace merutilm::vkh {
    class CommandBuffer final : public EngineHandler {
        const CommandPool &commandPool;
        std::vector<VkCommandBuffer> commandBuffers = {};

    public:
        explicit CommandBuffer(const Engine &engine, const CommandPool &commandPool);

        ~CommandBuffer() override;

        CommandBuffer(const CommandBuffer &) = delete;

        CommandBuffer &operator=(const CommandBuffer &) = delete;

        CommandBuffer(CommandBuffer &&) = delete;

        CommandBuffer &operator=(CommandBuffer &&) = delete;

        [[nodiscard]] VkCommandBuffer getCommandBufferHandle(const uint32_t frameIndex) const { return commandBuffers[frameIndex]; }

    private:
        void init() override;

        void destroy() override;
    };
}
