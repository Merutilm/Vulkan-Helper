//
// Created by Merutilm on 2025-07-20.
//

#include "TemplateShaderProgram.hpp"

#include "../repo/DescriptorRepo.hpp"
#include "../struct/Vertex.hpp"

namespace merutilm::vkh {
    void TemplateShaderProgram::update(const uint32_t frameIndex) {
        getPipeline().getPipelineManager().getDescriptor(0).getDescriptorManager().getUBO(DSTemplate::Time::UBO_TIME)
        .set(DSTemplate::Time::TIME_CURRENT, engine.getCore().getTime());
        getPipeline().getPipelineManager().getDescriptor(0).update(frameIndex);
    }

    void TemplateShaderProgram::draw(const VkCommandBuffer cbh, const uint32_t frameIndex) {
        const VkBuffer vertexBufferHandle = getVertexBuffer().getBufferHandle(frameIndex);
        constexpr VkDeviceSize vertexBufferOffset = 0;
        vkCmdBindVertexBuffers(cbh, 0, 1, &vertexBufferHandle, &vertexBufferOffset);
        getIndexBuffer().bind(cbh, frameIndex, "index");
        vkCmdDrawIndexed(cbh, getIndexBuffer().getElementCount("index"), 1, 0, 0, 0);
    }


    void TemplateShaderProgram::configurePipelineLayout(DescriptorSetLayoutRepo &layoutRepo,
                                                        PipelineLayoutManager &pipelineLayoutManager) {
        auto &layout = layoutRepo.pick(DescriptorSetLayoutName::UNIFORM_F1);
        auto bom = std::make_unique<BufferObjectManager>();
        bom->reserve<glm::vec2>("resolution");
        auto pushManager = std::make_unique<PushConstantManager>(VK_SHADER_STAGE_FRAGMENT_BIT, std::move(bom));
        pipelineLayoutManager.appendDescriptorSetLayout(&layout);
        pipelineLayoutManager.appendPushConstantManager("push", std::move(pushManager));
    }

    void TemplateShaderProgram::configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                     DescriptorSetLayoutRepo &layoutRepo, DescriptorRepo &repo) {
        descriptors.push_back(&repo.pick(DescriptorName::TIME, layoutRepo.pick(DescriptorSetLayoutName::UNIFORM_F1)));
    }

    std::unique_ptr<VertexBuffer> TemplateShaderProgram::createVertexBuffer() {
        auto bom = std::make_unique<BufferObjectManager>();
        const auto vbo = std::vector{
            Vertex::generate({1, 1, 0}, {1, 1, 1}),
            Vertex::generate({1, -1, 0}, {0, 1, 0}),
            Vertex::generate({-1, -1, 0}, {1, 0, 0}),
            Vertex::generate({-1, 1, 0}, {0, 0, 1}),
        };
        bom->add("square", vbo);
        return std::make_unique<VertexBuffer>(engine.getCore(), std::move(bom));
    }

    std::unique_ptr<IndexBuffer> TemplateShaderProgram::createIndexBuffer() {
        auto bom = std::make_unique<BufferObjectManager>();
        const auto ibo = std::vector<uint32_t>{0, 1, 2, 2, 3, 0};

        bom->add("index", ibo);
        return std::make_unique<IndexBuffer>(engine.getCore(), std::move(bom));
    }
}
