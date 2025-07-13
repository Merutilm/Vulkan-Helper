//
// Created by Merutilm on 2025-07-11.
//

#pragma once

#include "RenderPass.hpp"
#include "../manage/PipelineManager.hpp"

#include "../objhandler/EngineHandler.hpp"

namespace merutilm::vkh {
    class Pipeline final : public EngineHandler {
        VkPipeline pipeline = nullptr;
        const Descriptor &descriptor;
        const PipelineLayout &pipelineLayout;
        const RenderPass &renderPass;
        const std::string subpassName;
        const std::unique_ptr<PipelineManager> pipelineManager = nullptr;

    public:
        explicit Pipeline(const Engine &engine, const Descriptor &descriptor, const PipelineLayout &pipelineLayout,
                          const RenderPass &renderPass, const std::string &subpassName,
                          std::unique_ptr<PipelineManager> &&pipelineManager);

        ~Pipeline() override;

        Pipeline(const Pipeline &) = delete;

        Pipeline &operator=(const Pipeline &) = delete;

        Pipeline(Pipeline &&) = delete;

        Pipeline &operator=(Pipeline &&) = delete;

        [[nodiscard]] VkPipeline getPipelineHandle() const { return pipeline; }

    private:
        void init() override;

        void destroy() override;
    };
}
