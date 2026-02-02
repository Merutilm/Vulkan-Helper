//
// Created by Merutilm on 2026-02-03.
//

#include <vulkan_helper/manage/PipelineLayoutManager.hpp>
namespace merutilm::vkh {
    
    PipelineLayoutManagerImpl::PipelineLayoutManagerImpl() = default;

    PipelineLayoutManagerImpl::~PipelineLayoutManagerImpl() = default;

    bool PipelineLayoutManagerImpl::operator==(const PipelineLayoutManagerImpl &) const = default;

    void PipelineLayoutManagerImpl::appendDescriptorSetLayout(DescriptorSetLayoutPtr descriptorSetLayout) {
        builders.emplace_back(descriptorSetLayout);
        ++descriptorSetLayoutCount;
    }

    void PipelineLayoutManagerImpl::appendPushConstantManager(PushConstantPtr pushConstant) {
        builders.emplace_back(pushConstant);
    }
}