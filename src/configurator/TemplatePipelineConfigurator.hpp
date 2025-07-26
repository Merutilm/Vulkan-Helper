//
// Created by Merutilm on 2025-07-20.
//

#pragma once
#include "GeneralPipelineConfigurator.hpp"
#include "../repo/DescriptorRepo.hpp"

namespace merutilm::vkh {
    class TemplatePipelineConfigurator final : public GeneralPipelineConfigurator {
    public:
        using GeneralPipelineConfigurator::GeneralPipelineConfigurator;

        ~TemplatePipelineConfigurator() override = default;

        TemplatePipelineConfigurator(const TemplatePipelineConfigurator &) = delete;

        TemplatePipelineConfigurator &operator=(const TemplatePipelineConfigurator &) = delete;

        TemplatePipelineConfigurator(TemplatePipelineConfigurator &&) = delete;

        TemplatePipelineConfigurator &operator=(TemplatePipelineConfigurator &&) = delete;

        void update(VkCommandBuffer cbh, uint32_t frameIndex, uint32_t width, uint32_t height) override;


    protected:
        void configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                         DescriptorSetLayoutRepo &layoutRepo,
                                                         DescriptorRepo &repo) override;
        void configurePushConstant(DescriptorSetLayoutRepo &layoutRepo, PipelineLayoutManager &pipelineLayoutManager) override;


        void configureVertexBuffer(ShaderObjectManager &som) override;

        void configureIndexBuffer(ShaderObjectManager &som) override;
    };
}
