//
// Created by Merutilm on 2025-07-09.
//

#include "Swapchain.hpp"

#include "../exception/exception.hpp"
#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    Swapchain::Swapchain(const Surface &surface, const PhysicalDevice &physicalDevice,
                         const LogicalDevice &logicalDevice) : surface(surface), physicalDevice(physicalDevice),
                                                               logicalDevice(logicalDevice) {
        Swapchain::attach();
    }

    Swapchain::~Swapchain() {
        Swapchain::detach();
    }

    void Swapchain::recreate() {
        destroyImageViews();
        oldSwapchain = swapchain;
        createSwapchain(&swapchain, oldSwapchain);
        vkDestroySwapchainKHR(logicalDevice.getLogicalDeviceHandle(), oldSwapchain, nullptr);
        setupSwapchainImages();
    }


    VkExtent2D Swapchain::populateSwapchainExtent() const {
        const HWND window = surface.getTargetWindow().getWindowHandle();
        const auto capabilities = physicalDevice.populateSurfaceCapabilities();

        if (capabilities.currentExtent.width == UINT32_MAX) {
            return capabilities.currentExtent;
        }

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


    void Swapchain::attach() {
        createSwapchain(&swapchain, nullptr);
        setupSwapchainImages();
    }

    void Swapchain::createSwapchain(VkSwapchainKHR *target, const VkSwapchainKHR old) {
        const auto &[graphicsFamily, presentFamily] = physicalDevice.getQueueFamilyIndices();
        std::array queueFamilyIndices = {graphicsFamily.value(), presentFamily.value()};

        const VkSurfaceCapabilitiesKHR capabilities = physicalDevice.populateSurfaceCapabilities();

        swapchainImageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && swapchainImageCount > capabilities.maxImageCount) {
            swapchainImageCount = capabilities.maxImageCount;
        }

        if (const VkSwapchainCreateInfoKHR createInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = surface.getSurfaceHandle(),
            .minImageCount = swapchainImageCount,
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
            .oldSwapchain = old
        }; vkCreateSwapchainKHR(logicalDevice.getLogicalDeviceHandle(), &createInfo, nullptr, target) !=
           VK_SUCCESS) {
            throw exception_init("Failed to create swapchain!");
        }
    }

    void Swapchain::setupSwapchainImages() {
        swapchainImages.resize(swapchainImageCount);
        swapchainImageViews.resize(swapchainImageCount);

        vkGetSwapchainImagesKHR(logicalDevice.getLogicalDeviceHandle(), swapchain, &swapchainImageCount,
                                swapchainImages.data());
        for (uint32_t i = 0; i < swapchainImageCount; ++i) {
            BufferUtils::createImageView(logicalDevice.getLogicalDeviceHandle(), swapchainImages[i],
                                         VK_IMAGE_VIEW_TYPE_2D, 1, VK_FORMAT_R8G8B8A8_SRGB, &swapchainImageViews[i]);
        }
    }


    void Swapchain::detach() {
        destroyImageViews();
        vkDestroySwapchainKHR(logicalDevice.getLogicalDeviceHandle(), swapchain, nullptr);
    }

    void Swapchain::destroyImageViews() const {
        for (uint32_t i = 0; i < swapchainImageCount; ++i) {
            vkDestroyImageView(logicalDevice.getLogicalDeviceHandle(), swapchainImageViews[i], nullptr);
        }
    }
}
