//
// Created by Merutilm on 2025-07-09.
//

#pragma once
#include <vulkan_helper/core/Instance.hpp>
#include <vulkan_helper/core/PhysicalDeviceLoader.hpp>
#include <vulkan_helper/base/allocator.hpp>
#include <vulkan_helper/handle/Handler.hpp>

namespace merutilm::vkh {
    class LogicalDeviceImpl final : public Handler {
        InstanceRef instance;
        PhysicalDeviceLoaderRef physicalDevice;
        VkDevice logicalDevice = nullptr;
        VkQueue graphicsQueue = nullptr;
        VkQueue presentQueue = nullptr;

    public:
        explicit LogicalDeviceImpl(InstanceRef instance, PhysicalDeviceLoaderRef physicalDevice);

        ~LogicalDeviceImpl() override;

        LogicalDeviceImpl(const LogicalDeviceImpl &) = delete;

        LogicalDeviceImpl &operator=(const LogicalDeviceImpl &) = delete;

        LogicalDeviceImpl(LogicalDeviceImpl &&) = delete;

        LogicalDeviceImpl &operator=(LogicalDeviceImpl &&) = delete;

        [[nodiscard]] VkDevice getLogicalDeviceHandle() const { return logicalDevice; }

        [[nodiscard]] VkQueue getGraphicsQueue() const { return graphicsQueue; }

        [[nodiscard]] VkQueue getPresentQueue() const { return presentQueue; }

        void queueSubmit(const uint32_t submitCount, const VkSubmitInfo *pSubmits, const VkFence fence) {
            if (allocator::invoke(vkQueueSubmit, graphicsQueue, submitCount, pSubmits, fence) != VK_SUCCESS) {
                throw exception_invalid_state("Failed to submit queue!");
            }
        }

        void queuePresent(const VkPresentInfoKHR *presentInfo) {
            if (allocator::invoke(vkQueuePresentKHR,presentQueue, presentInfo) != VK_SUCCESS) {
                throw exception_invalid_state("Failed to present queue!");
            }
        }

        void waitDeviceIdle() {
            allocator::invoke(vkDeviceWaitIdle, logicalDevice);
        }

    private:
        void init() override;

        void destroy() override;
    };

    using LogicalDevice = std::unique_ptr<LogicalDeviceImpl>;
    using LogicalDevicePtr = LogicalDeviceImpl *;
    using LogicalDeviceRef = LogicalDeviceImpl &;
}
