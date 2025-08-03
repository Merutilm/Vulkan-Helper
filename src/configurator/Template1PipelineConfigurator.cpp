//
// Created by Merutilm on 2025-07-29.
//

#include "Template1PipelineConfigurator.hpp"

#include "../struct/Vertex.hpp"

namespace merutilm::vkh {




    void Template1PipelineConfigurator::updateQueue(DescriptorUpdateQueue &queue, const uint32_t frameIndex, const uint32_t imageIndex,
                                               const uint32_t width, const uint32_t height) {

        getDescriptor(0).updateQueue(queue, frameIndex);
    }

    void Template1PipelineConfigurator::render(const VkCommandBuffer cbh, const uint32_t frameIndex, const uint32_t width, const uint32_t height) {
        draw(cbh, frameIndex, 0);
    }


    void Template1PipelineConfigurator::configurePushConstant(DescriptorSetLayoutRepo &layoutRepo,
        PipelineLayoutManager &pipelineLayoutManager) {
        //noop
    }

    void Template1PipelineConfigurator::configureDescriptors(std::vector<const Descriptor *> &descriptors,
    DescriptorSetLayoutRepo &layoutRepo, SharedDescriptorRepo &descRepo) {
        auto texture = std::make_unique<Sampler2D>(engine.getCore(), engine.getCommandPool(), "../res/w.png");
        auto manager = std::make_unique<DescriptorManager>();
        manager->appendCombinedImgSampler(0, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(texture));
        appendUniqueDescriptor(0, descriptors, layoutRepo, std::move(manager));
    }

    void Template1PipelineConfigurator::configureVertexBuffer(ShaderObjectManager &som) {
        som.add(0, std::vector{
                    Vertex::generate({1, 1, 0}, {1, 1, 1}, {1, 1}),
                    Vertex::generate({1, -1, 0}, {1, 1, 1}, {1, 0}),
                    Vertex::generate({-1, -1, 0}, {1, 1, 1}, {0, 0}),
                    Vertex::generate({-1, 1, 0}, {1, 1, 1}, {0, 1}),
                });
    }

    void Template1PipelineConfigurator::configureIndexBuffer(ShaderObjectManager &som) {
        som.add(0, std::vector<uint32_t>{0, 1, 2, 2, 3, 0});
    }
}
