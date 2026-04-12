//
// Created by Merutilm on 2025-08-05.
//

#include <vulkan_helper/engine/configurator/GeneralPostProcessGraphicsPipelineConfigurator.hpp>

#include <vulkan_helper/base/vkh_base.hpp>
#include <vulkan_helper/engine/pipeline/GraphicsPipeline.hpp>
#include <vulkan_helper/engine/repo/GlobalPipelineLayoutRepo.hpp>
#include <vulkan_helper/engine/wrapped/Vertex.hpp>

namespace merutilm::mvk {
    void GeneralPostProcessGraphicsPipelineConfigurator::cmdRender(const VkCommandBuffer cbh, const uint32_t frameIndex,
                                                                   DescIndexPicker &&descIndices) {
        pipeline->cmdBindAll(cbh, frameIndex, std::move(descIndices));
        cmdPushAll(cbh);
        cmdDraw(cbh, frameIndex, 0);
    }


    void GeneralPostProcessGraphicsPipelineConfigurator::configureVertexBuffer(HostDataObjectManager &som) {
    }

    void GeneralPostProcessGraphicsPipelineConfigurator::configureIndexBuffer(HostDataObjectManager &som) {
    }

    void GeneralPostProcessGraphicsPipelineConfigurator::configure() {


        PipelineLayoutManager pipelineLayoutManager{};

        std::vector<Descriptor *> descriptors = {};
        configureDescriptors(descriptors);

        for (const auto descriptor: descriptors) {
            pipelineLayoutManager.appendDescriptorSetLayout(&descriptor->getLayout());
        }

        configurePushConstant(pipelineLayoutManager);
        PipelineLayout &pipelineLayout = engine.getGlobalRepositories().getRepository<GlobalPipelineLayoutRepo>()->pick(
                std::move(pipelineLayoutManager));


        PipelineManager pipelineManager(pipelineLayout);

        pipelineManager.attachDescriptor(std::move(descriptors));
        pipelineManager.attachShader(&vertexShader);
        pipelineManager.attachShader(&fragmentShader);

        pipeline = std::make_unique<GraphicsPipeline>(wc, pipelineLayout, vertexBufferStaticRef, indexBufferStaticRef,
            renderContextIndex, primarySubpassIndex,
            std::move(pipelineManager));

    }


    void GeneralPostProcessGraphicsPipelineConfigurator::createPPBuffer(WindowContext &wc, std::unique_ptr<VertexBuffer> & vertexBufferPP, std::unique_ptr<IndexBuffer> & indexBufferPP) {
        HostDataObjectManager vertManager;
        HostDataObjectManager indexManager;

        vertManager.addArray(0, std::vector{
                    Vertex::generate({1, 1, 0}, {1, 1, 1}, {1, 1}),
                    Vertex::generate({1, -1, 0}, {1, 1, 1}, {1, 0}),
                    Vertex::generate({-1, -1, 0}, {1, 1, 1}, {0, 0}),
                    Vertex::generate({-1, 1, 0}, {1, 1, 1}, {0, 1}),
            });
        indexManager.addArray(0, std::vector<uint32_t>{0, 1, 2, 2, 3, 0});

        vertexBufferPP = std::make_unique<VertexBuffer>(wc.core, std::move(vertManager), BufferLock::LOCK_ONLY, false);
        indexBufferPP = std::make_unique<IndexBuffer>(wc.core, std::move(indexManager), BufferLock::LOCK_ONLY, false);
        vertexBufferPP->update();
        indexBufferPP->update();
        vertexBufferPP->lock(wc.getCommandPool());
        indexBufferPP->lock(wc.getCommandPool());
    }
} // namespace merutilm::mvk
