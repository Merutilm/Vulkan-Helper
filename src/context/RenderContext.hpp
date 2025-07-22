//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <memory>

#include "../configurator/RenderPassConfigurator.hpp"
#include "../core/CommandBuffer.hpp"
#include "../core/Framebuffer.hpp"
#include "../core/RenderPass.hpp"
#include "../core/SyncObject.hpp"

namespace merutilm::vkh {

    struct RenderContext final {
        const std::string windowAttachmentName;
        std::unique_ptr<CommandBuffer> commandBuffer = nullptr;
        std::unique_ptr<SyncObject> syncObject = nullptr;
        std::unique_ptr<RenderPass> renderPass = nullptr;
        std::unique_ptr<Framebuffer> framebuffer = nullptr;

        explicit RenderContext(const Core &core, const std::string &windowAttachmentName,
                               const CommandPool &commandPool,
                               const RenderPassConfigurator &renderPassConfigurator) : windowAttachmentName(
            windowAttachmentName) {
            commandBuffer = std::make_unique<CommandBuffer>(core, commandPool);
            syncObject = std::make_unique<SyncObject>(core);

            auto renderPassManager = std::make_unique<RenderPassManager>();
            renderPassConfigurator.configure(*renderPassManager);
            renderPass = std::make_unique<RenderPass>(core, windowAttachmentName, std::move(renderPassManager));
            framebuffer = std::make_unique<Framebuffer>(core, windowAttachmentName, *renderPass);
        }
    };
}
