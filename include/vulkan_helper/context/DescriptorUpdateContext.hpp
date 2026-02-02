//
// Created by Merutilm on 2025-08-01.
//

#pragma once
#include <vulkan_helper/base/vkh_base.hpp>

namespace merutilm::vkh {
    struct DescriptorUpdateContext {
        VkWriteDescriptorSet writeSet;
        VkDescriptorBufferInfo bufferInfo;
        VkDescriptorImageInfo imageInfo;
    };

    using DescriptorUpdateQueue = std::list<DescriptorUpdateContext>;
}
