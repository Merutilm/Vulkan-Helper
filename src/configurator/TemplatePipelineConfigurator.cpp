//
// Created by Merutilm on 2025-07-20.
//

#include "TemplatePipelineConfigurator.hpp"

#include "../repo/DescriptorRepo.hpp"
#include "../struct/Vertex.hpp"

namespace merutilm::vkh {
    void TemplatePipelineConfigurator::update(const uint32_t frameIndex) {
        const Descriptor &desc0 = getPipeline().getPipelineManager().getDescriptor(0);
        desc0.getDescriptorManager().getUBO(DSTemplate::Time::UBO_TIME).set(
            DSTemplate::Time::TIME_CURRENT, engine.getCore().getTime());
        desc0.update(frameIndex);
    }

    void TemplatePipelineConfigurator::draw(const VkCommandBuffer cbh, const uint32_t frameIndex) {
        VFPipelineConfigurator::draw(cbh, frameIndex, "index");
    }


    void TemplatePipelineConfigurator::configurePipelineLayout(DescriptorSetLayoutRepo &layoutRepo,
                                                        PipelineLayoutManager &pipelineLayoutManager) {
        pipelineLayoutManager.appendDescriptorSetLayout(&layoutRepo.pick(DescriptorSetLayoutName::UNIFORM_F1));
        pipelineLayoutManager.appendPushConstantManager("push", VK_SHADER_STAGE_FRAGMENT_BIT,
            PushConstantReserve<glm::vec2>{"resolution"}
        );
    }

    void TemplatePipelineConfigurator::configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                     DescriptorSetLayoutRepo &layoutRepo, DescriptorRepo &repo) {
        descriptors.push_back(&repo.pick(DescriptorName::TIME, layoutRepo.pick(DescriptorSetLayoutName::UNIFORM_F1)));
    }

    void TemplatePipelineConfigurator::configureVertexBuffer(ShaderObjectManager &som) {
        som.add("square", std::vector{
                     Vertex::generate({1, 1, 0}, {1, 1, 1}),
                     Vertex::generate({1, -1, 0}, {0, 1, 0}),
                     Vertex::generate({-1, -1, 0}, {1, 0, 0}),
                     Vertex::generate({-1, 1, 0}, {0, 0, 1}),
                 });
    }

    void TemplatePipelineConfigurator::configureIndexBuffer(ShaderObjectManager &som) {
        som.add("index", std::vector<uint32_t>{0, 1, 2, 2, 3, 0});
    }
}
