//
// Created by Merutilm on 2025-09-05.
//



#pragma once
#include <vulkan-helper/base/pch.hpp>

namespace merutilm::vkh {
    struct BufferInitInfo {
        VkDeviceSize size;
        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;
    };


}
