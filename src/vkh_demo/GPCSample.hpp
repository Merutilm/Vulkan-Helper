//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include "vulkan_helper/engine/Engine.hpp"
#include "vulkan_helper/engine/configurator/GeneralPostProcessGraphicsPipelineConfigurator.hpp"
namespace merutilm::vkh_demo {

    class GPCSample final : public vkh::GeneralPostProcessGraphicsPipelineConfigurator {

    public:
        GPCSample(vkh::Engine &engine, vkh::WindowContext &wc) :
            GeneralPostProcessGraphicsPipelineConfigurator(engine, wc, "template-1.frag") {}

        void updateQueue(vkh::DescriptorUpdateQueue &queue, uint32_t frameIndex) override;
        void pipelineInitialized() override;
        void renderContextRefreshed() override;

    protected:
        void configurePushConstant(vkh::PipelineLayoutManager &pipelineLayoutManager) override;
        void configureDescriptors(std::vector<vkh::Descriptor *> &descriptors) override;
    };

} // namespace merutilm
