//
// Created by Merutilm on 2025-07-09.
//

#include "CommandPool.hpp"

#include "../exception/exception.hpp"

namespace merutilm::vkh {
    CommandPool::CommandPool(const Engine &engine) : EngineHandler(engine) {
        CommandPool::init();
    }

    CommandPool::~CommandPool() {
        CommandPool::destroy();
    }


    void CommandPool::init() {
        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
        commandPools.resize(maxFramesInFlight);
        for (int i = 0; i < maxFramesInFlight; ++i) {
            const VkCommandPoolCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = engine.getPhysicalDevice().getQueueFamilyIndices().graphicsFamily.
                value(),
            };
            if (vkCreateCommandPool(engine.getLogicalDevice().getLogicalDeviceHandle(), &createInfo, nullptr,
                                    &commandPools[i]) != VK_SUCCESS) {
                throw exception_init("Failed to create command pool!");
            }
        }
    }

    void CommandPool::destroy() {
        for (const auto commandPool : commandPools) {
            vkDestroyCommandPool(engine.getLogicalDevice().getLogicalDeviceHandle(), commandPool, nullptr);
        }
    }
}
