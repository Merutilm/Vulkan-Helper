//
// Created by Merutilm on 2025-07-14.
//

#pragma once
#include "Executor.hpp"
#include "../obj/CommandBuffer.hpp"
#include "../obj/Framebuffer.hpp"
#include "../obj/RenderPass.hpp"
#include "../obj/SyncObject.hpp"
#include "../handle/EngineHandler.hpp"

namespace merutilm::vkh {
    class CommandExecutor final : public Executor {
        const CommandBuffer &commandBuffer;
        const RenderPass &renderPass;
        const Framebuffer &framebuffer;
        const SyncObject &syncObject;
        uint32_t frameIndex = 0;
        uint32_t imageIndex = 0;

    public:
        explicit CommandExecutor(const Engine &engine, const CommandBuffer &commandBuffer, const RenderPass &renderPass,
                                 const Framebuffer &framebuffer, const SyncObject &syncObject, uint32_t frameIndex,
                                 uint32_t imageIndex);

        ~CommandExecutor() override;

        CommandExecutor(const CommandExecutor &) = delete;

        CommandExecutor &operator=(const CommandExecutor &) = delete;

        CommandExecutor(CommandExecutor &&) = delete;

        CommandExecutor &operator=(CommandExecutor &&) = delete;

        [[nodiscard]] VkFence getFence() const {
            return syncObject.getFence(frameIndex);
        };

        [[nodiscard]] VkSemaphore getImageAvailableSemaphore() const {
            return syncObject.getImageAvailableSemaphore(frameIndex);
        }
        [[nodiscard]] VkSemaphore getRenderFinishedSemaphore() const {
            return syncObject.getRenderFinishedSemaphore(frameIndex);
        }

        [[nodiscard]] VkCommandBuffer getCommandBufferHandle() const {
            return commandBuffer.getCommandBufferHandle(frameIndex);
        }

    private:
        void begin() override;

        void end() override;
    };
}
