//
// Created by Merutilm on 2025-07-18.
//

#include "BasicShaderProgram.hpp"

#include "../repo/DescriptorRepo.hpp"
#include "../repo/Repositories.hpp"

namespace merutilm::vkh {
    BasicShaderProgram::BasicShaderProgram(const Engine &engine,
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
        BasicShaderProgram::attach();
    }

    BasicShaderProgram::~BasicShaderProgram() {
        BasicShaderProgram::detach();
    }

    void BasicShaderProgram::configure() {
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
        vertexBuffer = createVertexBuffer();
        indexBuffer = createIndexBuffer();

        pipeline = std::make_unique<Pipeline>(engine, pipelineLayout, *vertexBuffer, *indexBuffer, subpassName,
                                              std::move(pipelineManager));

    }


    void BasicShaderProgram::attach() {
        //no operation
    }

    void BasicShaderProgram::detach() {
        pipeline = nullptr;
        indexBuffer = nullptr;
        vertexBuffer = nullptr;
    }


}
