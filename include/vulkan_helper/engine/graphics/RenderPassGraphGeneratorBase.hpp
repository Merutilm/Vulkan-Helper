//
// Created by Merutilm on 7/9/26.
//

#pragma once
#include <vulkan_helper/engine/manage/RenderPassManager.hpp>

namespace merutilm::vkh {


    class RenderPass;
    class GraphicsPipelineNode;
    class PipelineConfigurator;

    struct RenderPassGraphGeneratorBase {
        RenderPassManager rpm;
        std::vector<std::unique_ptr<GraphicsPipelineNode>> pipelines{};
        std::vector<std::unique_ptr<PipelineConfigurator>> pipelineConfigurators{};
        uint32_t createdSubpassesCount = 0;

        RenderPassGraphGeneratorBase();

        virtual ~RenderPassGraphGeneratorBase();

        void regenerate();

        virtual void createPipelines(RenderPass *rp) const = 0;

        void configureAll();

    protected:
        virtual void configureAttachments() = 0;

        virtual void configurePipelines() = 0;

        virtual void generate() = 0;

    };

} // namespace merutilm::vkh
