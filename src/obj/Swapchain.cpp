//
// Created by Merutilm on 2025-07-09.
//

#include "Swapchain.hpp"

#include "../exception/exception.hpp"

namespace merutilm::vkh {
    Swapchain::Swapchain(const Surface &surface, const PhysicalDevice &physicalDevice,
                         const LogicalDevice &logicalDevice) : surface(surface), physicalDevice(physicalDevice),
                                                               logicalDevice(logicalDevice) {
        Swapchain::init();
    }

    Swapchain::~Swapchain() {
        Swapchain::destroy();
    }


    VkExtent2D Swapchain::populateSwapchainExtent() const {
        const auto capabilities = physicalDevice.populateSurfaceCapabilities();
        if (capabilities.currentExtent.width == UINT32_MAX) {
            return capabilities.currentExtent;
        }

        const HWND window = surface.getTargetWindow().getWindowHandle();
        RECT rect;
        GetClientRect(window, &rect);
        const VkExtent2D extent = {
            .width = std::clamp(static_cast<uint32_t>(rect.right - rect.left), capabilities.minImageExtent.width,
                                capabilities.maxImageExtent.width),
            .height = std::clamp(static_cast<uint32_t>(rect.bottom - rect.top), capabilities.minImageExtent.height,
                                 capabilities.maxImageExtent.height),

        };
        return extent;
    }


    void Swapchain::init() {
        const auto &[graphicsFamily, presentFamily] = physicalDevice.getQueueFamilyIndices();
        std::array queueFamilyIndices = {graphicsFamily.value(), presentFamily.value()};

        const VkSurfaceCapabilitiesKHR capabilities = physicalDevice.populateSurfaceCapabilities();

        maxFramesInFlight = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && maxFramesInFlight > capabilities.maxImageCount) {
            maxFramesInFlight = capabilities.maxImageCount;
        }

        if (const VkSwapchainCreateInfoKHR createInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = surface.getSurfaceHandle(),
            .minImageCount = maxFramesInFlight,
            .imageFormat = VK_FORMAT_R8G8B8A8_SRGB,
            .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
            .imageExtent = populateSwapchainExtent(),
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = graphicsFamily == presentFamily
                                    ? VK_SHARING_MODE_EXCLUSIVE
                                    : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = graphicsFamily == presentFamily
                                         ? 0
                                         : static_cast<uint32_t>(queueFamilyIndices.size()),
            .pQueueFamilyIndices = graphicsFamily == presentFamily ? nullptr : queueFamilyIndices.data(),
            .preTransform = capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
            .clipped = VK_TRUE,
            .oldSwapchain = oldSwapchain
        }; vkCreateSwapchainKHR(logicalDevice.getLogicalDeviceHandle(), &createInfo, nullptr, &swapchain) !=
           VK_SUCCESS) {
            throw exception_init("Failed to create swapchain!");
        }
    }

    void Swapchain::destroy() {
        vkDestroySwapchainKHR(logicalDevice.getLogicalDeviceHandle(), swapchain, nullptr);
    }
}
