//
// Created by Merutilm on 2025-07-20.
//

#pragma once
#include "GeneralPipelineConfigurator.hpp"
#include "../repo/DescriptorRepo.hpp"

namespace merutilm::vkh {
    class Template2PipelineConfigurator final : public GeneralPipelineConfigurator {
    public:
        using GeneralPipelineConfigurator::GeneralPipelineConfigurator;

        ~Template2PipelineConfigurator() override = default;

        Template2PipelineConfigurator(const Template2PipelineConfigurator &) = delete;

        Template2PipelineConfigurator &operator=(const Template2PipelineConfigurator &) = delete;

        Template2PipelineConfigurator(Template2PipelineConfigurator &&) = delete;

        Template2PipelineConfigurator &operator=(Template2PipelineConfigurator &&) = delete;

        void update(uint32_t frameIndex, uint32_t imageIndex, uint32_t width, uint32_t height) override;

        void render(VkCommandBuffer cbh, uint32_t frameIndex, uint32_t width, uint32_t height) override;

    protected:
        void configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                         DescriptorSetLayoutRepo &layoutRepo,
                                                         DescriptorRepo &repo) override;
        void configurePushConstant(DescriptorSetLayoutRepo &layoutRepo, PipelineLayoutManager &pipelineLayoutManager) override;


        void configureVertexBuffer(ShaderObjectManager &som) override;

        void configureIndexBuffer(ShaderObjectManager &som) override;
    };
}
