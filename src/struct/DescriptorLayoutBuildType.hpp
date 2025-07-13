//
// Created by Merutilm on 2025-07-12.
//

#pragma once
#include <vulkan/vulkan.h>

namespace merutilm::vkh {
    struct DescriptorLayoutBuildType {
        VkDescriptorType type;
        VkShaderStageFlags stage;
    };
}
