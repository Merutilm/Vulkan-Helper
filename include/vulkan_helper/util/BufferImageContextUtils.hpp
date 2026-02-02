//
// Created by Merutilm on 2025-08-04.
//

#pragma once
#include <vulkan_helper/executor/ScopedNewCommandBufferExecutor.hpp>
#include <vulkan_helper/context/ImageContext.hpp>
#include <vulkan_helper/core/CommandPool.hpp>
#include <vulkan_helper/util/BufferImageUtils.hpp>
#include <vulkan_helper/util/BarrierUtils.hpp>
#include <vulkan_helper/context/BufferContext.hpp>

namespace merutilm::vkh {
    struct BufferImageContextUtils {
        static std::vector<VkImage> enumerateImages(const MultiframeImageContext &context);

        /**
        * Creates image from byte color array. result layout is <b>VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL.</b>
        */
        static ImageContext imageFromByteColorArray(CoreRef core, CommandPoolRef commandPool, const VkFormat format,
                                                    const uint32_t width, const uint32_t height,
                                                    const uint32_t texChannels, const uint32_t channelBits, const bool useMipmap,
                                                    const std::byte *const data);

        /**
        * Creates image from file path. result layout is <b>VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL.</b>
        */
        static ImageContext imageFromPath(CoreRef core, CommandPoolRef commandPool, const VkFormat format,
                                          const bool useMipmap,
                                          const std::string_view path);

        /**
         * Copies whole buffer.
         * @param commandBuffer The command buffer to record
         * @param srcBuffer The source buffer to copy.
         * @param dstBuffer The destination buffer to copy.
         */
        static void cmdCopyBuffer(const VkCommandBuffer commandBuffer, const BufferContext &srcBuffer,
                                         const BufferContext &dstBuffer);

        /**
         * Copies whole image to buffer.
         * @param commandBuffer The command buffer to record
         * @param image The source image to copy. its layout must be <b>VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL</b>
         * @param buffer The destination buffer to copy.
         */
        static void cmdCopyImageToBuffer(const VkCommandBuffer commandBuffer, const ImageContext &image,
                                         const BufferContext &buffer);


        /**
         * Generates the Mipmap. Input layout of image Context must be <b>VK_IMAGE_LAYOUT_TRANSFER_SRC_BIT</b>.
         */
        static void cmdGenerateMipmaps(const VkCommandBuffer commandBuffer, const ImageContext &imageContext,
                                       const VkImageLayout dstLayout);

        /**
        * Copy source image to destination image. srcContext layout must be <b>VK_IMAGE_LAYOUT_TRANSFER_SRC_BIT,</b> and dstContext layout must be <b>VK_IMAGE_LAYOUT_TRANSFER_DST_BIT.</b>
        */
        static void cmdCopyEqualSizeImage(const VkCommandBuffer commandBuffer, const uint32_t mipLevel,
                                          const ImageContext &srcContext, const ImageContext &dstContext);

        /**
        * Blit image to destination image. srcMipLevel layout must be <b>VK_IMAGE_LAYOUT_TRANSFER_SRC_BIT,</b> and dstMipLevel layout must be <b>VK_IMAGE_LAYOUT_TRANSFER_DST_BIT.</b>
        */
        static void cmdBlitImage(const VkCommandBuffer commandBuffer, const ImageContext &imageContext,
                                 const uint32_t srcMipLevel, const uint32_t dstMipLevel);
    };
}
