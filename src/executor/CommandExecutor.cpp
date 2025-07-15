//
// Created by Merutilm on 2025-07-14.
//

#include "CommandExecutor.hpp"

#include <iostream>

namespace merutilm::vkh {
    CommandExecutor::CommandExecutor(const Engine &engine, const CommandBuffer &commandBuffer,
                                     const RenderPass &renderPass, const Framebuffer &framebuffer,
                                     const SyncObject &syncObject,
                                     const uint32_t frameIndex, const uint32_t imageIndex): Executor(engine), commandBuffer(commandBuffer),
                                                                 renderPass(renderPass), framebuffer(framebuffer),
                                                                 syncObject(syncObject), frameIndex(frameIndex),
                                                                 imageIndex(imageIndex) {
        CommandExecutor::begin();
    }

    CommandExecutor::~CommandExecutor() {
        CommandExecutor::end();
    }

    void CommandExecutor::begin() {
        const VkCommandBuffer commandBufferHandle = getCommandBufferHandle();
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
    }

    void CommandExecutor::end() {
        std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSwapchainKHR swapchainHandle = engine.getSwapchain().getSwapchainHandle();
        VkCommandBuffer commandBufferHandle = getCommandBufferHandle();
        VkSemaphore imageAvailableSemaphore = getImageAvailableSemaphore();
        VkSemaphore renderFinishedSemaphore = getRenderFinishedSemaphore();

        vkEndCommandBuffer(commandBufferHandle);

        const VkSubmitInfo submitInfo = {
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

        if (vkQueueSubmit(engine.getLogicalDevice().getGraphicsQueue(), 1, &submitInfo, getFence()) != VK_SUCCESS) {
            std::cerr << "Failed to submit queue" << std::endl;
        }

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
