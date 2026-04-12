//
// Created by Merutilm on 2025-08-15.
//

#pragma once

#include <vulkan_helper/engine/context/DescriptorUpdateContext.hpp>

namespace merutilm::mvk {
    struct DescriptorUpdater {
        DescriptorUpdater() = delete;

        static DescriptorUpdateQueue createQueue();

        static void write(VkDevice device, const DescriptorUpdateQueue &queue);
    };


}
