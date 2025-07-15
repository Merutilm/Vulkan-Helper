//
// Created by Merutilm on 2025-07-09.
//

#include "CommandBuffer.hpp"

#include "../exception/exception.hpp"

namespace merutilm::vkh {
    CommandBuffer::CommandBuffer(const Engine &engine, const CommandPool &commandPool) : EngineHandler(engine), commandPool(commandPool) {
        CommandBuffer::init();
    }

    CommandBuffer::~CommandBuffer() {
        CommandBuffer::destroy();
    }


    void CommandBuffer::init() {
        const VkDevice device = engine.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
        commandBuffers.resize(maxFramesInFlight);
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            VkCommandBufferAllocateInfo allocInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext = nullptr,
                .commandPool = commandPool.getCommandPoolHandle(),
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1
            };
            if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffers[i]) != VK_SUCCESS) {
                throw exception_init("Failed to allocate command buffers!");
            }
        }
    }

    void CommandBuffer::destroy() {
        const VkDevice device = engine.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            vkFreeCommandBuffers(device, commandPool.getCommandPoolHandle(), 1, &commandBuffers[i]);
        }
    }



}
