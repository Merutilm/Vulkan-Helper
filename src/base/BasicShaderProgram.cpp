//
// Created by Merutilm on 2025-07-18.
//

#include "BasicShaderProgram.hpp"

#include "../repo/Repositories.hpp"

namespace merutilm::vkh {
    BasicShaderProgram::BasicShaderProgram(const Core &engine, Repositories *repositories,
                                           const RenderContext &renderContext,
                                           const std::string &subpassName,
                                           const std::string &vertName,
                                           const std::string &fragName) : CoreHandler(engine),
                                                                          repositories(repositories),
                                                                          renderContext(renderContext),
                                                                          subpassName(subpassName),
                                                                          vertexShader(
                                                                              repositories->getRepository<
                                                                                  ShaderModuleRepo>()->pick(vertName)),
                                                                          fragmentShader(
                                                                              repositories->getRepository<
                                                                                  ShaderModuleRepo>()->pick(fragName)) {
        BasicShaderProgram::attach();
    }

    BasicShaderProgram::~BasicShaderProgram() {
        BasicShaderProgram::detach();
    }

    void BasicShaderProgram::attach() {
        auto pipelineLayoutManager = std::make_unique<PipelineLayoutManager>();
        auto descLayoutManager = std::make_unique<DescriptorSetLayoutManager>();

        //do something logics

        // const DescriptorSetLayout &descriptorSetLayout = repositories->getRepository<DescriptorSetLayoutRepo>()->pick(
        //     std::move(descLayoutManager));
        // const PipelineLayout &pipelineLayout = repositories->getRepository<PipelineLayoutRepo>()->
        //         pick(std::move(pipelineLayoutManager));
        //
        // auto descManager = std::make_unique<DescriptorManager>(descriptorSetLayout);
        //
        // //do something logics
        //
        // auto descriptor = std::make_unique<Descriptor>(core, std::move(descManager));
        // auto pipelineManager = std::make_unique<PipelineManager>(pipelineLayout, std::move(descriptor));

        // TODO : create repository
        // pipeline = std::make_unique<Pipeline>(engine, pipelineLayout, *renderContext.renderPass, createVertexBuffer(), subpassName,
        //                                       std::move(pipelineManager));
    }

    void BasicShaderProgram::detach() {
        pipeline = nullptr;
    }
}
