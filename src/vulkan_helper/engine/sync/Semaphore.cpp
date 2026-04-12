//
// Created by Merutilm on 2025-09-01.
//

#include <vulkan_helper/engine/sync/Semaphore.hpp>

#include <vulkan_helper/base/allocator.hpp>

namespace merutilm::mvk {
    Semaphore::Semaphore(Core & core) : CoreHandler(core) {
        Semaphore::init();
    }

    Semaphore::~Semaphore() {
        Semaphore::cleanup();
    }

    void Semaphore::init() {

        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();

        constexpr VkSemaphoreCreateInfo semaphoreInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        };


        if (allocator::invoke(vkCreateSemaphore, device, &semaphoreInfo, nullptr, &imageAvailable) != VK_SUCCESS ||
            allocator::invoke(vkCreateSemaphore, device, &semaphoreInfo, nullptr, &renderFinished) != VK_SUCCESS) {
            throw exception_init("Failed to create sync objects!");
        }
    }

    void Semaphore::cleanup() {
        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        allocator::invoke(vkDestroySemaphore, device, imageAvailable, nullptr);
        allocator::invoke(vkDestroySemaphore, device, renderFinished, nullptr);
    }
}
