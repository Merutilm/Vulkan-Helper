//
// Created by Merutilm on 2025-07-18.
//

#include "RenderContext.hpp"

namespace merutilm::vkh {
    RenderContext::RenderContext(const Engine &engine, const std::string &windowAttachmentName,
                                 const CommandPool &commandPool, std::unique_ptr<RenderPassManager> &&renderPassManager) : windowAttachmentName(windowAttachmentName) {
        commandBuffer = std::make_unique<CommandBuffer>(engine, commandPool);
        syncObject = std::make_unique<SyncObject>(engine);
        renderPass = std::make_unique<RenderPass>(engine, windowAttachmentName, std::move(renderPassManager));
        framebuffer = std::make_unique<Framebuffer>(engine, windowAttachmentName, *renderPass);
    }


}
