//
// Created by Merutilm on 2025-07-09.
//

#pragma once
#include "../objhandler/Handler.hpp"

#include "LogicalDevice.hpp"

namespace merutilm::vkh {
    class Swapchain final : public Handler {

        const Surface &surface;
        const PhysicalDevice &physicalDevice;
        const LogicalDevice &logicalDevice;

        VkSwapchainKHR swapchain = nullptr;
        VkSwapchainKHR oldSwapchain = nullptr;
        uint32_t maxFramesInFlight = 0;

    public:
        explicit Swapchain(const Surface &surface, const PhysicalDevice &physicalDevice, const LogicalDevice &logicalDevice);

        ~Swapchain() override;

        Swapchain(const Swapchain &) = delete;

        Swapchain &operator=(const Swapchain &) = delete;

        Swapchain(Swapchain &&) = delete;

        Swapchain &operator=(Swapchain &&) = delete;

        [[nodiscard]] VkExtent2D populateSwapchainExtent() const;

        [[nodiscard]] VkSwapchainKHR getSwapchainHandle() const { return swapchain; }

        [[nodiscard]] uint32_t getMaxFramesInFlight() const { return maxFramesInFlight; }

    private:
        void init() override;

        void destroy() override;
    };
}
