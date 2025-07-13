//
// Created by Merutilm on 2025-07-11.
//

#pragma once
#include <memory>

#include "../manage/RenderPassManager.hpp"
#include "../objhandler/EngineHandler.hpp"

namespace merutilm::vkh {
    class RenderPass final : public EngineHandler {
        std::unique_ptr<RenderPassManager> manager = nullptr;
        VkRenderPass renderPass = nullptr;

    public:
        RenderPass(const Engine &engine, std::unique_ptr<RenderPassManager> &&manager);

        ~RenderPass() override;

        RenderPass(const RenderPass &) = delete;

        RenderPass &operator=(const RenderPass &) = delete;

        RenderPass(RenderPass &&) = delete;

        RenderPass &operator=(RenderPass &&) = delete;

        [[nodiscard]] const RenderPassManager &getManager() const { return *manager; }

        [[nodiscard]] VkRenderPass getRenderPassHandle() const { return renderPass; }

    private:
        void init() override;

        void destroy() override;
    };
}
