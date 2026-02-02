//
// Created by Merutilm on 2025-07-26.
//

#pragma once
#include <vulkan_helper/base/vkh_core.hpp>
#include <vulkan_helper/core/Swapchain.hpp>
#include <vulkan_helper/wrapped/ImageInitInfo.hpp>
#include <vulkan_helper/util/BufferImageUtils.hpp>

namespace merutilm::vkh {
    struct ImageContext;

    using MultiframeImageContext = std::vector<ImageContext>;

    struct ImageContext {
        VkImage image = VK_NULL_HANDLE;
        VkFormat imageFormat = VK_FORMAT_UNDEFINED;
        VkDeviceMemory imageMemory = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        VkImageView mipmappedImageView = VK_NULL_HANDLE;
        VkExtent2D extent = {};
        VkDeviceSize capacity = 0;

        static ImageContext createContext(CoreRef core, const ImageInitInfo &imageInitInfo);

        static MultiframeImageContext createMultiframeContext(CoreRef core, const ImageInitInfo &imageInitInfo);

        static void destroyContext(CoreRef core, const ImageContext & imgCtx);

        static void destroyContext(CoreRef core, const MultiframeImageContext & imgCtx);

        static MultiframeImageContext fromSwapchain(CoreRef core, SwapchainRef swapchain);
    };
}
