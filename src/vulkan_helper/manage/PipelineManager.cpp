//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/manage/PipelineManager.hpp>

namespace merutilm::vkh {
    PipelineManagerImpl::PipelineManagerImpl(PipelineLayoutRef layout): layout(layout) {
    }

    void PipelineManagerImpl::attachShader(ShaderModulePtr shaderStage) {
        shaderModules.emplace_back(shaderStage);
    }

    void PipelineManagerImpl::attachDescriptor(std::vector<DescriptorPtr> &&descriptor) { descriptors = std::move(descriptor); }
}