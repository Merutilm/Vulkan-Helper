//
// Created by Merutilm on 2025-09-05.
//



#pragma once
#include <vulkan_helper/base/vkh_base.hpp>

namespace merutilm::vkh {
    struct BufferInitInfo {
        VkDeviceSize size;
        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;
    };


}
