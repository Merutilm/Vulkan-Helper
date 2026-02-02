//
// Created by Merutilm on 2025-08-15.
//

#pragma once

#include <vulkan_helper/context/DescriptorUpdateContext.hpp>
#include <vulkan_helper/base/allocator.hpp>

namespace merutilm::vkh {
    struct DescriptorUpdater {
        DescriptorUpdater() = delete;

        static DescriptorUpdateQueue createQueue();

        static void write(const VkDevice device, const DescriptorUpdateQueue &queue);
    };


}
