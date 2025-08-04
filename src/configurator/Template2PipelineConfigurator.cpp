//
// Created by Merutilm on 2025-07-20.
//

#include "Template2PipelineConfigurator.hpp"

#include "BasicRenderContextConfigurator.hpp"
#include "../repo/SharedDescriptorRepo.hpp"
#include "../struct/Vertex.hpp"

namespace merutilm::vkh {
    void Template2PipelineConfigurator::updateQueue(DescriptorUpdateQueue &queue, const uint32_t frameIndex,
                                                    const uint32_t imageIndex,
                                                    const uint32_t width, const uint32_t height) {
        getVertexBuffer().update(frameIndex);
        getIndexBuffer().update(frameIndex);
        getDescriptor(SET_TIME).getDescriptorManager().get<std::unique_ptr<Uniform> >(DSSharedTemplate::Time::UBO_TIME)
                ->set(DSSharedTemplate::Time::TIME_CURRENT, engine.getCore().getTime());
        getDescriptor(SET_TIME).getDescriptorManager().get<std::unique_ptr<Uniform> >(DSSharedTemplate::Time::UBO_TIME)
                ->update(frameIndex);
        getDescriptor(SET_TIME).updateQueue(queue, frameIndex);

        getDescriptor(SET_INPUT_ATTACHMENT).getDescriptorManager().get<std::unique_ptr<
            Sampler2D> >(BINDING_INPUT_SAMPLER)->setImageContext(
            engine.getRenderContext().configurator->getImageContext(
                BasicRenderContextConfigurator::PRIMARY_SUBPASS_RESULT_IMAGE)[imageIndex]);
        getDescriptor(SET_INPUT_ATTACHMENT).getDescriptorManager().get<ImageContext>(BINDING_INPUT_ATTACHMENT_ELEM) =
                engine.getRenderContext().configurator->getImageContext(
                    BasicRenderContextConfigurator::PRIMARY_SUBPASS_RESULT_IMAGE)[imageIndex];
        getDescriptor(SET_INPUT_ATTACHMENT).updateQueue(queue, frameIndex);

        getPushConstantManager(PUSH_RESOLUTION).set(PUSH_RESOLUTION_SIZE, glm::vec2{
                                                        static_cast<float>(width), static_cast<float>(height)
                                                    });
    }

    void Template2PipelineConfigurator::render(const VkCommandBuffer cbh, const uint32_t frameIndex,
                                               const uint32_t width,
                                               const uint32_t height) {
        pushAll(cbh);
        draw(cbh, frameIndex, 0);
    }


    void Template2PipelineConfigurator::configureDescriptors(std::vector<const Descriptor *> &descriptors,
                                                             DescriptorSetLayoutRepo &layoutRepo,
                                                             SharedDescriptorRepo &repo) {
        appendDescriptor(SET_TIME, descriptors, layoutRepo, repo, DescriptorName::TIME, VK_SHADER_STAGE_FRAGMENT_BIT);
        auto inputManager = std::make_unique<DescriptorManager>();
        inputManager->appendCombinedImgSampler(BINDING_INPUT_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT,
                                               std::make_unique<Sampler2D>(engine.getCore()));
        inputManager->appendInputAttachment(BINDING_INPUT_ATTACHMENT_ELEM, VK_SHADER_STAGE_FRAGMENT_BIT);
        appendUniqueDescriptor(SET_INPUT_ATTACHMENT, descriptors, layoutRepo, std::move(inputManager));
    }

    void Template2PipelineConfigurator::configurePushConstant(DescriptorSetLayoutRepo &layoutRepo,
                                                              PipelineLayoutManager &pipelineLayoutManager) {
        pipelineLayoutManager.appendPushConstantManager(PUSH_RESOLUTION, VK_SHADER_STAGE_FRAGMENT_BIT,
                                                        PushConstantReserve<glm::vec2>{PUSH_RESOLUTION_SIZE}
        );
    }

    void Template2PipelineConfigurator::configureVertexBuffer(ShaderObjectManager &som) {
        som.add(0, std::vector{
                    Vertex::generate({1, 1, 0}, {1, 1, 1}, {1, 1}),
                    Vertex::generate({1, -1, 0}, {0, 1, 0}, {1, 0}),
                    Vertex::generate({-1, -1, 0}, {1, 0, 0}, {0, 0}),
                    Vertex::generate({-1, 1, 0}, {0, 0, 1}, {0, 1}),
                });
    }

    void Template2PipelineConfigurator::configureIndexBuffer(ShaderObjectManager &som) {
        som.add(0, std::vector<uint32_t>{0, 1, 2, 2, 3, 0});
    }
}
