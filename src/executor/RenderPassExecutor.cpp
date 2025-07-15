//
// Created by Merutilm on 2025-07-15.
//

#include "RenderPassExecutor.hpp"

namespace merutilm::vkh {
    RenderPassExecutor::RenderPassExecutor(const Engine &engine,
                                           const CommandBuffer &commandBuffer,const RenderPass &renderPass,
                                           const Framebuffer &framebuffer, const uint32_t frameIndex,
                                           const uint32_t imageIndex) : Executor(engine),
                                                                        commandBuffer(commandBuffer),renderPass(renderPass),
                                                                        framebuffer(framebuffer),
                                                                        frameIndex(frameIndex), imageIndex(imageIndex) {
        RenderPassExecutor::begin();
    }

    RenderPassExecutor::~RenderPassExecutor() {
        RenderPassExecutor::end();
    }


    void RenderPassExecutor::begin() {
        std::array<VkClearValue, 2> clearValues = {};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        const VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = renderPass.getRenderPassHandle(),
            .framebuffer = framebuffer.getFramebufferHandle(imageIndex),
            .renderArea = {
                .offset = {0, 0},
                .extent = engine.getSwapchain().populateSwapchainExtent()
            },
            .clearValueCount = static_cast<uint32_t>(clearValues.size()),
            .pClearValues = clearValues.data()
        };

        vkCmdBeginRenderPass(commandBuffer.getCommandBufferHandle(frameIndex), &renderPassBeginInfo,
                             VK_SUBPASS_CONTENTS_INLINE);
    }

    void RenderPassExecutor::end() {
        vkCmdEndRenderPass(commandBuffer.getCommandBufferHandle(frameIndex));
    }
}
