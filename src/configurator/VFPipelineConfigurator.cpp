//
// Created by Merutilm on 2025-07-18.
//

#include "VFPipelineConfigurator.hpp"

#include "../repo/DescriptorRepo.hpp"
#include "../repo/Repositories.hpp"

namespace merutilm::vkh {
    VFPipelineConfigurator::VFPipelineConfigurator(const Engine &engine,
                                           const std::string &subpassName,
                                           const std::string &vertName,
                                           const std::string &fragName) : EngineHandler(engine),
                                                                          subpassName(subpassName),
                                                                          vertexShader(
                                                                              engine.getRepositories().getRepository<
                                                                                  ShaderModuleRepo>()->pick(vertName)),
                                                                          fragmentShader(
                                                                              engine.getRepositories().getRepository<
                                                                                  ShaderModuleRepo>()->pick(fragName)) {
        VFPipelineConfigurator::attach();
    }

    VFPipelineConfigurator::~VFPipelineConfigurator() {
        VFPipelineConfigurator::detach();
    }

    void VFPipelineConfigurator::draw(const VkCommandBuffer cbh, const uint32_t frameIndex, const std::string &indexVarname) const {
        const VkBuffer vertexBufferHandle = getVertexBuffer().getBufferHandle(frameIndex);
        constexpr VkDeviceSize vertexBufferOffset = 0;
        vkCmdBindVertexBuffers(cbh, 0, 1, &vertexBufferHandle, &vertexBufferOffset);
        getIndexBuffer().bind(cbh, frameIndex, indexVarname);
        vkCmdDrawIndexed(cbh, getIndexBuffer().getElementCount(indexVarname), 1, 0, 0, 0);
    }


    void VFPipelineConfigurator::configure() {
        auto pipelineLayoutManager = std::make_unique<PipelineLayoutManager>();
        auto &layoutRepo = *engine.getRepositories().getRepository<DescriptorSetLayoutRepo>();
        auto &descRepo = *engine.getRepositories().getRepository<DescriptorRepo>();

        configurePipelineLayout(layoutRepo, *pipelineLayoutManager);
        const PipelineLayout &pipelineLayout = engine.getRepositories().getRepository<PipelineLayoutRepo>()->pick(std::move(pipelineLayoutManager));


        auto pipelineManager = std::make_unique<PipelineManager>(pipelineLayout);

        std::vector<const Descriptor *> descriptors = {};
        configureDescriptors(descriptors, layoutRepo, descRepo);

        pipelineManager->attachDescriptor(std::move(descriptors));
        pipelineManager->attachShader(&vertexShader);
        pipelineManager->attachShader(&fragmentShader);

        auto vertManager = std::make_unique<ShaderObjectManager>();
        auto indexManager = std::make_unique<ShaderObjectManager>();

        configureVertexBuffer(*vertManager);
        configureIndexBuffer(*indexManager);

        vertexBuffer = std::make_unique<VertexBuffer>(engine.getCore(), std::move(vertManager));
        indexBuffer = std::make_unique<IndexBuffer>(engine.getCore(), std::move(indexManager));

        pipeline = std::make_unique<Pipeline>(engine, pipelineLayout, *vertexBuffer, *indexBuffer, subpassName,
                                              std::move(pipelineManager));

    }


    void VFPipelineConfigurator::attach() {
        //no operation
    }

    void VFPipelineConfigurator::detach() {
        pipeline = nullptr;
        indexBuffer = nullptr;
        vertexBuffer = nullptr;
    }


}
