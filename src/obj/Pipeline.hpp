//
// Created by Merutilm on 2025-07-11.
//

#pragma once

#include "RenderPass.hpp"
#include "VertexBuffer.hpp"
#include "../manage/PipelineManager.hpp"

#include "../handle/EngineHandler.hpp"

namespace merutilm::vkh {
    class Pipeline final : public EngineHandler {
        VkPipeline pipeline = nullptr;
        const PipelineLayout &pipelineLayout;
        const RenderPass &renderPass;
        const VertexBuffer &vertexBuffer;
        const std::string subpassName;
        const std::unique_ptr<PipelineManager> pipelineManager = nullptr;

    public:
        explicit Pipeline(const Engine &engine, const PipelineLayout &pipelineLayout,
                          const RenderPass &renderPass,
                          const VertexBuffer &vertexBuffer,
                          std::string subpassName,
                          std::unique_ptr<PipelineManager> &&pipelineManager);

        ~Pipeline() override;

        Pipeline(const Pipeline &) = delete;

        Pipeline &operator=(const Pipeline &) = delete;

        Pipeline(Pipeline &&) = delete;

        Pipeline &operator=(Pipeline &&) = delete;

        [[nodiscard]] const PipelineManager &getPipelineManager() const { return *pipelineManager; }

        [[nodiscard]] PipelineManager &getPipelineManager() {return *pipelineManager;}

        [[nodiscard]] VkPipeline getPipelineHandle() const { return pipeline; }

    private:
        void attach() override;

        void detach() override;
    };
}
