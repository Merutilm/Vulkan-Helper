//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <memory>

#include "../obj/CommandBuffer.hpp"
#include "../obj/Framebuffer.hpp"
#include "../obj/RenderPass.hpp"
#include "../obj/SyncObject.hpp"

namespace merutilm::vkh {
    struct RenderContext final {

        const std::string windowAttachmentName;
        std::unique_ptr<CommandBuffer> commandBuffer = nullptr;
        std::unique_ptr<SyncObject> syncObject = nullptr;
        std::unique_ptr<RenderPass> renderPass = nullptr;
        std::unique_ptr<Framebuffer> framebuffer = nullptr;

        explicit RenderContext(const Engine &engine, const std::string &windowAttachmentName, const CommandPool &commandPool, std::unique_ptr<RenderPassManager> &&renderPassManager);

    };
}
