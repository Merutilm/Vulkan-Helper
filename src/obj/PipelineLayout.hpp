//
// Created by Merutilm on 2025-07-13.
//

#pragma once
\
#include "../handle/EngineHandler.hpp"
#include "../manage/PipelineLayoutManager.hpp"

namespace merutilm::vkh {
    class PipelineLayout final : public EngineHandler {

        std::unique_ptr<PipelineLayoutManager> pipelineLayoutManager = nullptr;
        VkPipelineLayout layout = nullptr;

    public:
        explicit PipelineLayout(const Engine &engine, std::unique_ptr<PipelineLayoutManager> &&pipelineLayoutManager);

        ~PipelineLayout() override;

        PipelineLayout(const PipelineLayout &) = delete;

        PipelineLayout &operator=(const PipelineLayout &) = delete;

        PipelineLayout(PipelineLayout &&) = delete;

        PipelineLayout &operator=(PipelineLayout &&) = delete;

        void push(VkCommandBuffer commandBuffer) const;

        [[nodiscard]] VkPipelineLayout getLayoutHandle() const { return layout; }

    private:
        void attach() override;

        void detach() override;
    };
}
