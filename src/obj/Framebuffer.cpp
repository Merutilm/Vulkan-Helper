//
// Created by Merutilm on 2025-07-14.
//

#include "Framebuffer.hpp"

#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    Framebuffer::Framebuffer(const Engine &engine, const RenderPass &renderPass) : EngineHandler(engine),
        renderPass(renderPass) {
        Framebuffer::init();
    }

    Framebuffer::~Framebuffer() {
        Framebuffer::destroy();
    }

    void Framebuffer::init() {
        const uint32_t swapchainImages = engine.getSwapchain().getSwapchainImageCount();
        const auto [width, height] = engine.getSwapchain().populateSwapchainExtent();
        framebuffer.resize(swapchainImages);
        auto& attachments = renderPass.getManager().getAttachments();

        for (uint32_t i = 0; i < swapchainImages; ++i) {
            auto imageViews = std::vector<VkImageView>(attachments.size());
            std::ranges::transform(attachments, imageViews.begin(), [i](const RenderPassAttachment &v){return v.imageViews[i];});

            const VkFramebufferCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .renderPass = renderPass.getRenderPassHandle(),
                .attachmentCount = static_cast<uint32_t>(imageViews.size()),
                .pAttachments = imageViews.data(),
                .width = width,
                .height = height,
                .layers = 1
            };


            if (vkCreateFramebuffer(engine.getLogicalDevice().getLogicalDeviceHandle(), &createInfo, nullptr,
                                    &framebuffer[i]) != VK_SUCCESS) {
                throw exception_init("Failed to create framebuffer");
            }
        }
    }

    void Framebuffer::destroy() {
        const VkDevice device = engine.getLogicalDevice().getLogicalDeviceHandle();
        for (uint32_t i = 0; i < engine.getSwapchain().getSwapchainImageCount(); ++i) {
            vkDestroyFramebuffer(device, framebuffer[i], nullptr);
        }
    }
}
