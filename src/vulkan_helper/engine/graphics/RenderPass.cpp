//
// Created by Merutilm on 2025-07-11.
//

#include <vulkan_helper/base/exception.hpp>
#include <vulkan_helper/engine/graphics/RenderPass.hpp>
#include <vulkan_helper/engine/manage/RenderPassManager.hpp>

namespace merutilm::vkh {
    RenderPass::RenderPass(Core &core, std::unique_ptr<RenderPassManager> &&manager) :
        CoreHandler(core), attachments(std::move(manager->attachments)),
        preserveIndices(std::move(manager->preserveIndices)),
        attachmentReferences(std::move(manager->attachmentReferences)),
        subpassDependencies(std::move(manager->subpassDependencies)), subpassCount(manager->subpassCount) {
        RenderPass::init();
    }

    RenderPass::~RenderPass() { RenderPass::cleanup(); }

    void RenderPass::init() {
        const uint32_t subpasses = getSubpassCount();
        std::vector<VkSubpassDescription> subpassDescription(subpasses);

        auto &dependencies = getSubpassDependencies();

        using enum RenderPassAttachmentType;
        for (uint32_t i = 0; i < subpasses; i++) {
            auto &inputRef = getAttachmentReferences(i, INPUT);
            auto &colorRef = getAttachmentReferences(i, COLOR);
            auto &resolveRef = getAttachmentReferences(i, RESOLVE);
            auto &depthStencilRef = getAttachmentReferences(i, DEPTH_STENCIL);

#ifndef NDEBUG
            if (!resolveRef.empty() && colorRef.size() != resolveRef.size()) {
                throw exception_init(std::format(
                        "SUBPASS {}: the size of color attachment and resolve attachment doesn't match ", i));
            }
#endif
            subpassDescription[i] = {
                    .flags = 0,
                    .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                    .inputAttachmentCount = static_cast<uint32_t>(inputRef.size()),
                    .pInputAttachments = inputRef.size() == 0 ? nullptr : inputRef.data(),
                    .colorAttachmentCount = static_cast<uint32_t>(colorRef.size()),
                    .pColorAttachments = colorRef.size() == 0 ? nullptr : colorRef.data(),
                    .pResolveAttachments = colorRef.size() == 0 ? nullptr : resolveRef.data(),
                    .pDepthStencilAttachment = colorRef.size() == 0 ? nullptr : depthStencilRef.data(),
                    .preserveAttachmentCount = static_cast<uint32_t>(preserveIndices[i].size()),
                    .pPreserveAttachments = preserveIndices[i].size() == 0 ? nullptr : preserveIndices[i].data(),
            };
        }
        auto &attachments = getAttachments();
        auto attachmentDesc = std::vector<VkAttachmentDescription>(attachments.size());
        std::ranges::transform(attachments, attachmentDesc.begin(),
                               [](const std::unique_ptr<RenderPassAttachment> &v) { return v->attachment; });
        const VkRenderPassCreateInfo renderPassInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .attachmentCount = static_cast<uint32_t>(attachmentDesc.size()),
                .pAttachments = attachmentDesc.size() == 0  ? nullptr : attachmentDesc.data(),
                .subpassCount = static_cast<uint32_t>(subpassDescription.size()),
                .pSubpasses = subpassDescription.size() == 0 ? nullptr : subpassDescription.data(),
                .dependencyCount = static_cast<uint32_t>(dependencies.size()),
                .pDependencies = dependencies.size() == 0 ? nullptr : dependencies.data(),
        };
        if (vkCreateRenderPass(core.getLogicalDevice().getLogicalDeviceHandle(), &renderPassInfo, nullptr,
                               &renderPass) != VK_SUCCESS) {
            throw exception_init("Failed to create render pass!");
        }
    }

    void RenderPass::cleanup() {
        vkDestroyRenderPass(core.getLogicalDevice().getLogicalDeviceHandle(), renderPass, nullptr);
    }
} // namespace merutilm::vkh
