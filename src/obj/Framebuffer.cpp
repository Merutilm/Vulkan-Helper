//
// Created by Merutilm on 2025-07-14.
//

#include "Framebuffer.hpp"

#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    Framebuffer::Framebuffer(const Core &core, const std::string &windowAttachmentName,
                             const RenderPass &renderPass) : CoreHandler(core),
                                                             swapchain(
                                                                 *core.getWindowContext(windowAttachmentName).
                                                                 swapchain),
                                                             renderPass(renderPass) {
        Framebuffer::attach();
    }

    Framebuffer::~Framebuffer() {
        Framebuffer::detach();
    }

    void Framebuffer::attach() {
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        const auto [width, height] = swapchain.populateSwapchainExtent();
        framebuffer.resize(maxFramesInFlight);
        auto &attachments = renderPass.getAttachments();


        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            auto attachmentImageViews = std::vector<VkImageView>(attachments.size());
            std::ranges::transform(attachments, attachmentImageViews.begin(), [i](const RenderPassAttachment &v) {
                return v.imageViews[i];
            });

            const VkFramebufferCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .renderPass = renderPass.getRenderPassHandle(),
                .attachmentCount = static_cast<uint32_t>(attachmentImageViews.size()),
                .pAttachments = attachmentImageViews.data(),
                .width = width,
                .height = height,
                .layers = 1
            };


            if (vkCreateFramebuffer(core.getLogicalDevice().getLogicalDeviceHandle(), &createInfo, nullptr,
                                    &framebuffer[i]) != VK_SUCCESS) {
                throw exception_init("Failed to create framebuffer");
            }
        }
    }

    void Framebuffer::detach() {
        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            vkDestroyFramebuffer(device, framebuffer[i], nullptr);
        }
    }
}
