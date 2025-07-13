//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include "Descriptor.hpp"
#include "CommandBuffer.hpp"
#include "../manage/PushConstantManager.hpp"
#include "../objhandler/EngineHandler.hpp"

namespace merutilm::vkh {
    class PipelineLayout final : public EngineHandler {
        const Descriptor &descriptor;
        const PushConstantManager &pushConstantManager;
        VkPipelineLayout layout = nullptr;

    public:
        explicit PipelineLayout(const Engine &engine, const Descriptor &descriptor, const PushConstantManager &pushConstantManager);

        ~PipelineLayout() override;

        PipelineLayout(const PipelineLayout &) = delete;

        PipelineLayout &operator=(const PipelineLayout &) = delete;

        PipelineLayout(PipelineLayout &&) = delete;

        PipelineLayout &operator=(PipelineLayout &&) = delete;

        void push(const CommandBuffer &commandBuffer, uint32_t frameIndex) const;

        [[nodiscard]] VkPipelineLayout getLayoutHandle() const { return layout ; }

    private:
        void init() override;

        void destroy() override;
    };
}
