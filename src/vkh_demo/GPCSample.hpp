//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include "vulkan_helper/engine/configurator/GeneralPostProcessGraphicsPipelineConfigurator.hpp"
namespace merutilm::mvk_demo {

    class GPCSample final : public mvk::GeneralPostProcessGraphicsPipelineConfigurator {

    public:
        GPCSample(mvk::Engine &engine, const uint32_t windowContextIndex, const uint32_t renderContextIndex,
                  const uint32_t primarySubpassIndex, mvk::VertexBuffer &vertexBufferStaticRef, mvk::IndexBuffer &indexBufferStaticRef) :
            GeneralPostProcessGraphicsPipelineConfigurator(engine, windowContextIndex, renderContextIndex,
                                                           primarySubpassIndex, "template-1.frag", vertexBufferStaticRef, indexBufferStaticRef) {}

        void updateQueue(mvk::DescriptorUpdateQueue &queue, uint32_t frameIndex) override;
        void pipelineInitialized() override;
        void renderContextRefreshed() override;

    protected:
        void configurePushConstant(mvk::PipelineLayoutManager &pipelineLayoutManager) override;
        void configureDescriptors(std::vector<mvk::Descriptor *> &descriptors) override;
    };

} // namespace merutilm
