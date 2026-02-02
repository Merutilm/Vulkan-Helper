//
// Created by Merutilm on 2025-09-03.
//

#pragma once
#include <vulkan_helper/base/vkh_base.hpp>

namespace merutilm::vkh {
    struct BarrierUtils {
        static void cmdMemoryBarrier(const VkCommandBuffer commandBuffer, const VkAccessFlags srcAccessMask,
                                     const VkAccessFlags dstAccessMask, const VkPipelineStageFlags srcStageMask,
                                     const VkPipelineStageFlags dstStageMask);

        static void cmdBufferMemoryBarrier(const VkCommandBuffer commandBuffer, const VkAccessFlags srcAccessMask,
                                           const VkAccessFlags dstAccessMask, const VkBuffer buffer,
                                           const VkDeviceSize offset,
                                           const VkDeviceSize size,
                                           const VkPipelineStageFlags srcStageMask,
                                           const VkPipelineStageFlags dstStageMask);

        static void cmdSynchronizeImageWriteToRead(const VkCommandBuffer commandBuffer, const VkImage image,
                                                   const VkImageLayout currentLayout, const uint32_t mipLevel, const uint32_t mipLevelCount,
                                                   const VkPipelineStageFlags srcStageMask,
                                                   const VkPipelineStageFlags dstStageMask);

        static void cmdImageMemoryBarrier(const VkCommandBuffer commandBuffer, const VkImage image,
                                          const VkAccessFlags srcAccessMask,
                                          const VkAccessFlags dstAccessMask,
                                          const VkImageLayout oldLayout,
                                          const VkImageLayout newLayout,
                                          const uint32_t mipLevel,
                                          const uint32_t mipLevelCount,
                                          const VkPipelineStageFlags srcStageMask,
                                          const VkPipelineStageFlags dstStageMask);
    };
}
