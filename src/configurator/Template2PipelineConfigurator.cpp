//
// Created by Merutilm on 2025-07-20.
//

#include "Template2PipelineConfigurator.hpp"

#include <iostream>

#include "BasicRenderContextConfigurator.hpp"
#include "../repo/DescriptorRepo.hpp"
#include "../struct/Vertex.hpp"

namespace merutilm::vkh {
    void Template2PipelineConfigurator::updateQueue(DescriptorUpdateQueue &queue, const uint32_t frameIndex, const uint32_t imageIndex,
                                               const uint32_t width, const uint32_t height) {
        const Descriptor &desc0 = getDescriptor(0);

        const auto& ubo = desc0.getDescriptorManager().get<std::unique_ptr<Uniform>>(DSTemplate::Time::UBO_TIME);
        ubo->set(DSTemplate::Time::TIME_CURRENT, engine.getCore().getTime());
        ubo->update(frameIndex);

        desc0.updateQueue(queue, frameIndex);

        const auto &context = engine.getRenderContext().configurator->getImageContext(BasicRenderContextConfigurator::PRIMARY_SECONDARY_LINK_FRAME_IMG_CONTEXT)[imageIndex];
        getDescriptor(1).getDescriptorManager().get<ImageContext>("i") = context;
        getDescriptor(1).updateQueue(queue, frameIndex);

        getPushConstantManager("push").set("resolution", glm::vec2{
                                               static_cast<float>(width), static_cast<float>(height)
                                           });
    }

    void Template2PipelineConfigurator::render(const VkCommandBuffer cbh, const uint32_t frameIndex, const uint32_t width,
                                               const uint32_t height) {
        pushAll(cbh);
        draw(cbh, frameIndex, "index");
    }


    void Template2PipelineConfigurator::configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                             DescriptorSetLayoutRepo &layoutRepo,
                                                             DescriptorRepo &repo) {
        descriptors.push_back(&repo.pick(DescriptorName::TIME, layoutRepo, VK_SHADER_STAGE_FRAGMENT_BIT));
        auto inputManager = std::make_unique<DescriptorManager>();
        inputManager->appendInputAttachment("i", VK_SHADER_STAGE_FRAGMENT_BIT);
        appendUniqueDescriptor(descriptors, layoutRepo, std::move(inputManager));
    }

    void Template2PipelineConfigurator::configurePushConstant(DescriptorSetLayoutRepo &layoutRepo,
                                                              PipelineLayoutManager &pipelineLayoutManager) {
        pipelineLayoutManager.appendPushConstantManager("push", VK_SHADER_STAGE_FRAGMENT_BIT,
                                                        PushConstantReserve<glm::vec2>{"resolution"}
        );
    }

    void Template2PipelineConfigurator::configureVertexBuffer(ShaderObjectManager &som) {
        som.add("square", std::vector{
                    Vertex::generate({1, 1, 0}, {1, 1, 1}, {1, 1}),
                    Vertex::generate({1, -1, 0}, {0, 1, 0}, {1, 0}),
                    Vertex::generate({-1, -1, 0}, {1, 0, 0}, {0.25, 0.25}),
                    Vertex::generate({-1, 1, 0}, {0, 0, 1}, {0, 1}),
                });
    }

    void Template2PipelineConfigurator::configureIndexBuffer(ShaderObjectManager &som) {
        som.add("index", std::vector<uint32_t>{0, 1, 2, 2, 3, 0});
    }
}
