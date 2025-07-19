//
// Created by Merutilm on 2025-07-09.
//

#include "CommandPool.hpp"

#include "../exception/exception.hpp"

namespace merutilm::vkh {
    CommandPool::CommandPool(const Core &core) : CoreHandler(core) {
        CommandPool::attach();
    }

    CommandPool::~CommandPool() {
        CommandPool::detach();
    }


    void CommandPool::attach() {
        const VkCommandPoolCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = core.getPhysicalDevice().getQueueFamilyIndices().graphicsFamily.
            value(),
        };
        if (vkCreateCommandPool(core.getLogicalDevice().getLogicalDeviceHandle(), &createInfo, nullptr, &commandPool)
            != VK_SUCCESS) {
            throw exception_init("Failed to create command pool!");
        }
    }

    void CommandPool::detach() {
        vkDestroyCommandPool(core.getLogicalDevice().getLogicalDeviceHandle(), commandPool, nullptr);
    }
}
