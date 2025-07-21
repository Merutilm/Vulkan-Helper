//
// Created by Merutilm on 2025-07-20.
//

#pragma once
#include "BasicShaderProgram.hpp"
#include "../repo/DescriptorRepo.hpp"

namespace merutilm::vkh {
    class TemplateShaderProgram final : public BasicShaderProgram {
    public:
        using BasicShaderProgram::BasicShaderProgram;

        ~TemplateShaderProgram() override = default;

        TemplateShaderProgram(const TemplateShaderProgram &) = delete;

        TemplateShaderProgram &operator=(const TemplateShaderProgram &) = delete;

        TemplateShaderProgram(TemplateShaderProgram &&) = delete;

        TemplateShaderProgram &operator=(TemplateShaderProgram &&) = delete;

        void update(uint32_t frameIndex) override;

        void draw(VkCommandBuffer cbh, uint32_t frameIndex) override;

    protected:
        void configurePipelineLayout(DescriptorSetLayoutRepo &layoutRepo, PipelineLayoutManager &pipelineLayoutManager) override;

        void configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                         DescriptorSetLayoutRepo &layoutRepo,
                                                         DescriptorRepo &repo) override;

        std::unique_ptr<VertexBuffer> createVertexBuffer() override;

        std::unique_ptr<IndexBuffer> createIndexBuffer() override;
    };
}
