//
// Created by Merutilm on 2025-07-14.
//

#pragma once
#include "RenderPass.hpp"
#include "../handle/EngineHandler.hpp"

namespace merutilm::vkh {
    class Framebuffer final : public EngineHandler {
        const Swapchain &swapchain;
        const RenderPass &renderPass;
        std::vector<VkFramebuffer> framebuffer = {};

    public:
        explicit Framebuffer(const Engine &engine, const std::string &windowAttachmentName, const RenderPass &renderPass);

        ~Framebuffer() override;

        Framebuffer(const Framebuffer &) = delete;

        Framebuffer &operator=(const Framebuffer &) = delete;

        Framebuffer(Framebuffer &&) = delete;

        Framebuffer &operator=(Framebuffer &&) = delete;

        [[nodiscard]] VkFramebuffer getFramebufferHandle(const uint32_t imageIndex) const { return framebuffer[imageIndex]; }

        void attach() override;

        void detach() override;
    };
}
