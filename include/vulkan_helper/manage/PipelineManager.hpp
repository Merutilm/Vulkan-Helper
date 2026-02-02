//
// Created by Merutilm on 2025-07-11.
//

#pragma once
#include <vulkan_helper/base/vkh_base.hpp>
#include <vulkan_helper/core/Descriptor.hpp>
#include <vulkan_helper/core/PipelineLayout.hpp>
#include <vulkan_helper/core/ShaderModule.hpp>

namespace merutilm::vkh {
    struct PipelineManagerImpl {
        PipelineLayoutRef layout;
        std::vector<DescriptorPtr> descriptors = {};
        std::vector<ShaderModulePtr> shaderModules = {};

        explicit PipelineManagerImpl(PipelineLayoutRef layout);


        void attachShader(ShaderModulePtr shaderStage);

        void attachDescriptor(std::vector<DescriptorPtr> &&descriptor);
    };



    using PipelineManager = std::unique_ptr<PipelineManagerImpl>;
    using PipelineManagerPtr = PipelineManagerImpl *;
    using PipelineManagerRef = PipelineManagerImpl &;
}
