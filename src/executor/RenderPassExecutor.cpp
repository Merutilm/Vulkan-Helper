//
// Created by Merutilm on 2025-07-15.
//

#include "RenderPassExecutor.hpp"

#include <iostream>

#include "../context/RenderContext.hpp"

namespace merutilm::vkh {
    RenderPassExecutor::RenderPassExecutor(const Core &engine, const RenderContext &renderContext,
                                           const uint32_t frameIndex,
                                           const uint32_t imageIndex) : Executor(engine),
                                                                        swapchain(*engine.getWindowContext(
                                                                                renderContext.windowAttachmentName).
                                                                            swapchain),
                                                                        renderContext(renderContext),
                                                                        frameIndex(frameIndex), imageIndex(imageIndex) {
        RenderPassExecutor::begin();
    }

    RenderPassExecutor::~RenderPassExecutor() {
        RenderPassExecutor::end();
    }


    void RenderPassExecutor::begin() {
        const VkCommandBuffer commandBufferHandle = renderContext.commandBuffer->getCommandBufferHandle(frameIndex);
        constexpr VkCommandBufferBeginInfo beginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr
        };
        vkResetCommandBuffer(commandBufferHandle, 0);
        if (vkBeginCommandBuffer(commandBufferHandle, &beginInfo) != VK_SUCCESS) {
            throw exception_init("Failed to begin command buffer transmission");
        }

        std::array<VkClearValue, 2> clearValues = {};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        const VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = renderContext.renderPass->getRenderPassHandle(),
            .framebuffer = renderContext.framebuffer->getFramebufferHandle(imageIndex),
            .renderArea = {
                .offset = {0, 0},
                .extent = swapchain.populateSwapchainExtent()
            },
            .clearValueCount = static_cast<uint32_t>(clearValues.size()),
            .pClearValues = clearValues.data()
        };

        vkCmdBeginRenderPass(renderContext.commandBuffer->getCommandBufferHandle(frameIndex), &renderPassBeginInfo,
                             VK_SUBPASS_CONTENTS_INLINE);
    }

    void RenderPassExecutor::end() {
        vkCmdEndRenderPass(renderContext.commandBuffer->getCommandBufferHandle(frameIndex));


        VkCommandBuffer commandBufferHandle = renderContext.commandBuffer->getCommandBufferHandle(frameIndex);
        VkSemaphore imageAvailableSemaphore = renderContext.syncObject->getImageAvailableSemaphore(frameIndex);
        VkSemaphore renderFinishedSemaphore = renderContext.syncObject->getRenderFinishedSemaphore(frameIndex);
        std::array<VkPipelineStageFlags, 1> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        vkEndCommandBuffer(commandBufferHandle);


        if (const VkSubmitInfo submitInfo = {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .pNext = nullptr,
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = &imageAvailableSemaphore,
                .pWaitDstStageMask = waitStages.data(),
                .commandBufferCount = 1,
                .pCommandBuffers = &commandBufferHandle,
                .signalSemaphoreCount = 1,
                .pSignalSemaphores = &renderFinishedSemaphore
            };
            vkQueueSubmit(engine.getLogicalDevice().getGraphicsQueue(), 1, &submitInfo,
                          renderContext.syncObject->getFence(frameIndex)) != VK_SUCCESS) {
            std::cerr << "Failed to submit queue" << std::endl;
        }

        VkSwapchainKHR swapchainHandle = swapchain.getSwapchainHandle();
        const VkPresentInfoKHR presentInfo = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &renderFinishedSemaphore,
            .swapchainCount = 1,
            .pSwapchains = &swapchainHandle,
            .pImageIndices = &imageIndex,
            .pResults = nullptr
        };
        if (vkQueuePresentKHR(engine.getLogicalDevice().getPresentQueue(), &presentInfo) != VK_SUCCESS) {
            std::cerr << "Failed to present queue" << std::endl;
        }
    }
}
