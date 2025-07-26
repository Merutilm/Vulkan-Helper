//
// Created by Merutilm on 2025-07-20.
//

#include "TemplatePipelineConfigurator.hpp"

#include "../repo/DescriptorRepo.hpp"
#include "../struct/Vertex.hpp"

namespace merutilm::vkh {
    void TemplatePipelineConfigurator::update(const VkCommandBuffer cbh, const uint32_t frameIndex,
                                              const uint32_t width, const uint32_t height) {
        const Descriptor &desc0 = getPipeline().getPipelineManager().getDescriptor(0);
        desc0.getDescriptorManager().getUBO(DSTemplate::Time::UBO_TIME).set(
            DSTemplate::Time::TIME_CURRENT, engine.getCore().getTime());
        desc0.update(frameIndex);
        const Descriptor &desc1 = getPipeline().getPipelineManager().getDescriptor(1);
        desc1.update(frameIndex);

        getPipeline().getPipelineManager().getLayout().getPipelineLayoutManager().getPCM("push").set(
            "resolution", glm::vec2{static_cast<float>(width), static_cast<float>(height)});
        getPipeline().getPipelineManager().getLayout().push(cbh);

        draw(cbh, frameIndex, "index");
    }


    void TemplatePipelineConfigurator::configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                            DescriptorSetLayoutRepo &layoutRepo, DescriptorRepo &repo) {
        descriptors.push_back(&repo.pick(DescriptorName::TIME, layoutRepo, VK_SHADER_STAGE_FRAGMENT_BIT));
        auto texture = std::make_unique<TextureSampler>(engine.getCore(), engine.getCommandPool(), "../res/w.png");
        auto manager = std::make_unique<DescriptorManager>();
        manager->appendCombinedImgSampler("tex", VK_SHADER_STAGE_FRAGMENT_BIT, std::move(texture));
        descriptors.push_back(appendUniqueDescriptor(layoutRepo, std::move(manager)));
    }

    void TemplatePipelineConfigurator::configurePushConstant(DescriptorSetLayoutRepo &layoutRepo,
                                                             PipelineLayoutManager &pipelineLayoutManager) {
        pipelineLayoutManager.appendPushConstantManager("push", VK_SHADER_STAGE_FRAGMENT_BIT,
                                                        PushConstantReserve<glm::vec2>{"resolution"}
        );
    }

    void TemplatePipelineConfigurator::configureVertexBuffer(ShaderObjectManager &som) {
        som.add("square", std::vector{
                    Vertex::generate({1, 1, 0}, {1, 1, 1}, {1, 1}),
                    Vertex::generate({1, -1, 0}, {0, 1, 0}, {1, 0}),
                    Vertex::generate({-1, -1, 0}, {1, 0, 0}, {0.25, 0.25}),
                    Vertex::generate({-1, 1, 0}, {0, 0, 1}, {0, 1}),
                });
    }

    void TemplatePipelineConfigurator::configureIndexBuffer(ShaderObjectManager &som) {
        som.add("index", std::vector<uint32_t>{0, 1, 2, 2, 3, 0});
    }
}
