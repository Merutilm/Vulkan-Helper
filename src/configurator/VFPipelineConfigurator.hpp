//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <string>

#include "../context/RenderContext.hpp"
#include "../handle/EngineHandler.hpp"
#include "../core/Pipeline.hpp"
#include "../core/ShaderModule.hpp"
#include "../repo/DescriptorRepo.hpp"
#include "../repo/Repositories.hpp"


namespace merutilm::vkh {
    class VFPipelineConfigurator : public EngineHandler {
        const std::string subpassName;
        const ShaderModule &vertexShader;
        const ShaderModule &fragmentShader;
        std::unique_ptr<Pipeline> pipeline = nullptr;
        std::unique_ptr<VertexBuffer> vertexBuffer = {};
        std::unique_ptr<IndexBuffer> indexBuffer = nullptr;

    public:
        explicit VFPipelineConfigurator(const Engine &engine,
                                    const std::string &subpassName,
                                    const std::string &vertName,
                                    const std::string &fragName);

        ~VFPipelineConfigurator() override;

        VFPipelineConfigurator(const VFPipelineConfigurator &) = delete;

        VFPipelineConfigurator(VFPipelineConfigurator &&) = delete;

        VFPipelineConfigurator &operator=(const VFPipelineConfigurator &) = delete;

        VFPipelineConfigurator &operator=(VFPipelineConfigurator &&) = delete;

        template<typename ProgramName> requires std::is_base_of_v<VFPipelineConfigurator, ProgramName>
        static std::unique_ptr<ProgramName> createShaderProgram(const Engine &engine,
                                                          const std::string &subpassName,
                                                          const std::string &vertName,
                                                          const std::string &fragName) {
            auto shaderProgram = std::make_unique<ProgramName>(engine, subpassName, vertName, fragName);
            shaderProgram->configure();
            return shaderProgram;
        }

        Pipeline &getPipeline() const { return *pipeline; }

        const VertexBuffer &getVertexBuffer() const { return *vertexBuffer; }

        const IndexBuffer &getIndexBuffer() const { return *indexBuffer; }

        virtual void update(uint32_t frameIndex) = 0;

        virtual void draw(VkCommandBuffer cbh, uint32_t frameIndex) = 0;

    protected:
        void draw(VkCommandBuffer cbh, uint32_t frameIndex, const std::string &indexVarname) const;

        virtual void configure();

        virtual void configurePipelineLayout(DescriptorSetLayoutRepo &layoutRepo, PipelineLayoutManager &pipelineLayoutManager) = 0;

        virtual void configureDescriptors(std::vector<const Descriptor *> &descriptors, DescriptorSetLayoutRepo &layoutRepo, DescriptorRepo &descRepo) = 0;

        virtual void configureVertexBuffer(ShaderObjectManager &som) = 0;

        virtual void configureIndexBuffer(ShaderObjectManager &som) = 0;

    private:
        void attach() override;

        void detach() override;
    };
}
