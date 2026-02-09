//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include "vulkan_helper/engine/configurator/GeneralPostProcessGraphicsPipelineConfigurator.hpp"
namespace merutilm {

    class GPCSample final : public vkh::GeneralPostProcessGraphicsPipelineConfigurator {

    public:
        GPCSample(vkh::Engine &engine, const uint32_t windowContextIndex, const uint32_t renderContextIndex,
                  const uint32_t primarySubpassIndex) :
            GeneralPostProcessGraphicsPipelineConfigurator(engine, windowContextIndex, renderContextIndex,
                                                           primarySubpassIndex, "template-1.frag") {}

        void updateQueue(vkh::DescriptorUpdateQueue &queue, uint32_t frameIndex) override;
        void pipelineInitialized() override;
        void renderContextRefreshed() override;

    protected:
        void configurePushConstant(vkh::PipelineLayoutManager &pipelineLayoutManager) override;
        void configureDescriptors(std::vector<vkh::Descriptor *> &descriptors) override;
    };

} // namespace merutilm
