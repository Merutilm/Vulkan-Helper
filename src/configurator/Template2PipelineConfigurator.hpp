//
// Created by Merutilm on 2025-07-20.
//

#pragma once
#include "GeneralPipelineConfigurator.hpp"
#include "../repo/SharedDescriptorRepo.hpp"

namespace merutilm::vkh {
    class Template2PipelineConfigurator final : public GeneralPipelineConfigurator {

        static constexpr uint32_t SET_TIME = 0;
        static constexpr uint32_t SET_INPUT_ATTACHMENT = 1;

        static constexpr uint32_t BINDING_INPUT_ATTACHMENT_ELEM = 0;

        static constexpr uint32_t PUSH_RESOLUTION = 0;
        static constexpr uint32_t PUSH_RESOLUTION_SIZE = 0;

    public:
        using GeneralPipelineConfigurator::GeneralPipelineConfigurator;

        ~Template2PipelineConfigurator() override = default;

        Template2PipelineConfigurator(const Template2PipelineConfigurator &) = delete;

        Template2PipelineConfigurator &operator=(const Template2PipelineConfigurator &) = delete;

        Template2PipelineConfigurator(Template2PipelineConfigurator &&) = delete;

        Template2PipelineConfigurator &operator=(Template2PipelineConfigurator &&) = delete;

        void updateQueue(DescriptorUpdateQueue &queue, uint32_t frameIndex, uint32_t imageIndex, uint32_t width, uint32_t height) override;

        void render(VkCommandBuffer cbh, uint32_t frameIndex, uint32_t width, uint32_t height) override;

    protected:
        void configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                         DescriptorSetLayoutRepo &layoutRepo,
                                                         SharedDescriptorRepo &repo) override;
        void configurePushConstant(DescriptorSetLayoutRepo &layoutRepo, PipelineLayoutManager &pipelineLayoutManager) override;


        void configureVertexBuffer(ShaderObjectManager &som) override;

        void configureIndexBuffer(ShaderObjectManager &som) override;
    };
}
