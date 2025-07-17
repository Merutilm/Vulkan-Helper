//
// Created by Merutilm on 2025-07-14.
//

#pragma once
#include <vulkan/vulkan.h>

#include "ImageInitInfo.hpp"
#include "RenderPassAttachmentType.hpp"

namespace merutilm::vkh {
    struct RenderPassAttachmentConfig {
        RenderPassAttachmentType type;
        VkAttachmentDescription attachment;
        ImageInitInfo imageInitInfo;
    };
}
