//
// Created by Merutilm on 8/29/26.
//

#pragma once
#include "vulkan-helper/engine/context/WindowContext.hpp"
#include "vulkan-helper/engine/descriptor/Descriptor.hpp"
namespace merutilm::vkh {

    struct DescriptorTemplateManager {

        WindowContext &wc;
        Descriptor & desc;

        explicit DescriptorTemplateManager(WindowContext &wc,  Descriptor &target) : wc(wc), desc(target) {

        }

    };

}