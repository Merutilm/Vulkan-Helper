//
// Created by Merutilm on 2025-07-11.
//

#pragma once
#include <memory>

#include "../manage/RenderPassManager.hpp"
#include "../handle/EngineHandler.hpp"
#include "../struct/RenderPassAttachment.hpp"

namespace merutilm::vkh {
    class RenderPass final : public EngineHandler {
        std::unique_ptr<RenderPassManager> manager = nullptr;
        std::vector<RenderPassAttachment> attachments = {};
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

        [[nodiscard]] const std::vector<RenderPassAttachment> &getAttachments() const {
            return attachments;
        }

        void attach() override;

        void detach() override;
    };
}
