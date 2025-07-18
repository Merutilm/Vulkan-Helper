//
// Created by Merutilm on 2025-07-15.
//

#pragma once
#include "Executor.hpp"
#include "../context/RenderContext.hpp"
#include "../handle/EngineHandler.hpp"

namespace merutilm::vkh {
    class RenderPassExecutor final : public Executor {
        const RenderContext &renderContext;
        const uint32_t frameIndex;
        const uint32_t imageIndex;

    public:
        explicit RenderPassExecutor(const Engine &engine, const RenderContext &renderContext,  uint32_t frameIndex, uint32_t imageIndex);

        ~RenderPassExecutor() override;

        RenderPassExecutor(const RenderPassExecutor &) = delete;

        RenderPassExecutor &operator=(const RenderPassExecutor &) = delete;

        RenderPassExecutor(RenderPassExecutor &&) = delete;

        RenderPassExecutor &operator=(RenderPassExecutor &&) = delete;

    private:
        void begin() override;

        void end() override;
    };
}
