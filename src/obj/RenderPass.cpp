//
// Created by Merutilm on 2025-07-11.
//

#include "RenderPass.hpp"

#include <format>
#include <ranges>

#include "../exception/exception.hpp"
#include "../manage/RenderPassManager.hpp"

namespace merutilm::vkh {
    RenderPass::RenderPass(const Core &core, const std::string &windowAttachmentName,
    std::unique_ptr<RenderPassManager> &&manager) : CoreHandler(core), swapchain(*core.getWindowContext(windowAttachmentName).swapchain),
                                                                           manager(std::move(manager)) {
        RenderPass::attach();
    }

    RenderPass::~RenderPass() {
        RenderPass::detach();
    }

    void RenderPass::attach() {
        const uint32_t subpasses = manager->getSubpassCount();
        std::vector<VkSubpassDescription> subpassDescription(subpasses);
        const auto &attachmentConfigs = manager->getAttachmentConfigs();

        auto &dependencies = manager->getSubpassDependencies();
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();

        attachments.clear();

        for (auto [type, attachment, imageInitInfo]: attachmentConfigs) {
            std::vector<VkImage> images(maxFramesInFlight);
            std::vector<VkDeviceMemory> memories(maxFramesInFlight);
            std::vector<VkImageView> imageViews(maxFramesInFlight);
            if (type == RenderPassAttachmentType::COLOR) {
                auto swapchainImages = swapchain.getSwapchainImages();
                auto swapchainImageViews = swapchain.getSwapchainImageViews();
                std::ranges::copy(swapchainImages, images.begin());
                std::ranges::copy(swapchainImageViews, imageViews.begin());
            } else {
                const auto [width, height] = swapchain.populateSwapchainExtent();
                imageInitInfo.extent = VkExtent3D(width, height, 1);

                for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
                    BufferUtils::initImage(core, imageInitInfo, &images[i],
                                           &memories[i],
                                           &imageViews[i]);
                }
            }

            attachments.emplace_back(type, attachment, images, memories, imageViews);
        }


        using enum RenderPassAttachmentType;
        for (uint32_t i = 0; i < subpasses; i++) {
            auto &inputRef = manager->getAttachmentReferences(i, INPUT);
            auto &colorRef = manager->getAttachmentReferences(i, COLOR);
            auto &resolveRef = manager->getAttachmentReferences(i, RESOLVE);
            auto &depthStencilRef = manager->getAttachmentReferences(i, DEPTH_STENCIL);


            if (!resolveRef.empty() && colorRef.size() != resolveRef.size()) {
                throw exception_init(
                    std::format("SUBPASS {}: the size of color attachment and resolve attachment doesn't match ", i));
            }
            subpassDescription[i] = {
                .flags = 0,
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .inputAttachmentCount = static_cast<uint32_t>(inputRef.size()),
                .pInputAttachments = inputRef.data(),
                .colorAttachmentCount = static_cast<uint32_t>(colorRef.size()),
                .pColorAttachments = colorRef.data(),
                .pResolveAttachments = resolveRef.data(),
                .pDepthStencilAttachment = depthStencilRef.data(),
                .preserveAttachmentCount = manager->getPreserveIndicesCount(i),
                .pPreserveAttachments = manager->getPreserveIndices(i),
            };
        }

        auto attachmentDesc = std::vector<VkAttachmentDescription>(attachmentConfigs.size());
        std::ranges::transform(attachmentConfigs, attachmentDesc.begin(),
                               [](const RenderPassAttachmentConfig &v) { return v.attachment; });
        const VkRenderPassCreateInfo renderPassInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .attachmentCount = static_cast<uint32_t>(attachmentConfigs.size()),
            .pAttachments = attachmentDesc.data(),
            .subpassCount = subpasses,
            .pSubpasses = subpassDescription.data(),
            .dependencyCount = static_cast<uint32_t>(dependencies.size()),
            .pDependencies = dependencies.data(),
        };
        if (vkCreateRenderPass(core.getLogicalDevice().getLogicalDeviceHandle(), &renderPassInfo, nullptr,
                               &renderPass) !=
            VK_SUCCESS) {
            throw exception_init("Failed to create render pass!");
        }
    }

    void RenderPass::detach() {
        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        for (auto &attachment: attachments) {
            for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
                if (attachment.type == RenderPassAttachmentType::COLOR) {
                    continue;
                }
                vkDestroyImageView(device, attachment.imageViews[i], nullptr);
                vkDestroyImage(device, attachment.images[i], nullptr);
                vkFreeMemory(device, attachment.imageMemories[i], nullptr);
            }
        }
        vkDestroyRenderPass(core.getLogicalDevice().getLogicalDeviceHandle(), renderPass, nullptr);
    }
}
