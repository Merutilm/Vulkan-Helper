//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <vulkan_helper/core/PushConstant.hpp>
#include <vulkan_helper/core/DescriptorSetLayout.hpp>

namespace merutilm::vkh {

    using PipelineLayoutBuildType = std::variant<DescriptorSetLayoutPtr, PushConstantPtr>;
    using PipelineLayoutBuilder = std::vector<PipelineLayoutBuildType>;

    struct PipelineLayoutManagerImpl {
        PipelineLayoutBuilder builders;
        uint32_t descriptorSetLayoutCount = 0;

        PipelineLayoutManagerImpl();

        ~PipelineLayoutManagerImpl();

        PipelineLayoutManagerImpl(const PipelineLayoutManagerImpl &) = delete;

        PipelineLayoutManagerImpl &operator=(const PipelineLayoutManagerImpl &) = delete;

        PipelineLayoutManagerImpl(PipelineLayoutManagerImpl &&) = delete;

        PipelineLayoutManagerImpl &operator=(PipelineLayoutManagerImpl &&) = delete;

        bool operator==(const PipelineLayoutManagerImpl &) const;

        void appendDescriptorSetLayout(DescriptorSetLayoutPtr descriptorSetLayout);

        void appendPushConstantManager(PushConstantPtr pushConstant);
    };


    using PipelineLayoutManager = std::unique_ptr<PipelineLayoutManagerImpl>;
    using PipelineLayoutManagerPtr = PipelineLayoutManagerImpl *;
    using PipelineLayoutManagerRef = PipelineLayoutManagerImpl &;
}
