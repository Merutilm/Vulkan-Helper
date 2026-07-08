//
// Created by Merutilm on 7/8/26.
//

#pragma once
#include "RenderPassAttachment.hpp"
#include "RenderPassAttachmentType.hpp"
#include "vulkan_helper/util/BufferImageContextUtils.hpp"
namespace merutilm::vkh {
    struct RenderPassAttachmentReference {
        const RenderPassAttachment * targetAttachment;
        const RenderPassAttachmentType attachmentType;
        const VkPipelineStageFlags pipelineStageFlags;
        const VkAccessFlags accessFlags;
        const VkDependencyFlags dependencyFlags;
        const VkImageLayout layoutToUse;
    };
} // namespace merutilm::vkh
