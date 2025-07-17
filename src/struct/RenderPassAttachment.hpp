//
// Created by Merutilm on 2025-07-14.
//

#pragma once
#include <vulkan/vulkan.h>

#include "RenderPassAttachmentConfig.hpp"
#include <vector>

namespace merutilm::vkh {
    struct RenderPassAttachment {
        RenderPassAttachmentType type;
        VkAttachmentDescription attachment;
        std::vector<VkImage> images;
        std::vector<VkDeviceMemory> imageMemories;
        std::vector<VkImageView> imageViews;
    };
}
