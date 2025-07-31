//
// Created by Merutilm on 2025-07-22.
//

#pragma once
#include "RenderContextConfigurator.hpp"

namespace merutilm::vkh {
    struct BasicRenderContextConfigurator final : RenderContextConfigurator {
        inline static const std::string SUBPASS_PRIMARY = "Subpass";
        inline static const std::string SUBPASS_SECONDARY = "Subpass2";
        inline static const std::string ATTACHMENT_PRIMARY_COL = "primary_color";
        inline static const std::string ATTACHMENT_SECONDARY_INPUT = "secondary_input";
        inline static const std::string ATTACHMENT_SECONDARY_COLOR = "secondary_color";
        inline static const std::string ATTACHMENT_DEPTH = "depth_stencil";
        inline static const std::string PRIMARY_SECONDARY_LINK_FRAME_IMG_CONTEXT = "ic1";

        explicit BasicRenderContextConfigurator(const Core &core, const Swapchain &swapchain) : RenderContextConfigurator(core, swapchain) {

        }

        void configureImageContext() override {
            const auto [width, height] = swapchain.populateSwapchainExtent();

            contexts[PRIMARY_SECONDARY_LINK_FRAME_IMG_CONTEXT] =
                    ImageContext::init(core, ImageInitInfo{
                                           .imageType = VK_IMAGE_TYPE_2D,
                                           .imageViewType = VK_IMAGE_VIEW_TYPE_2D,
                                           .imageFormat = VK_FORMAT_R8G8B8A8_SRGB,
                                           .extent = {width, height, 1},
                                           .mipLevels = 1,
                                           .arrayLayers = 1,
                                           .samples = VK_SAMPLE_COUNT_1_BIT,
                                           .imageTiling = VK_IMAGE_TILING_OPTIMAL,
                                           .usage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |
                                                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                           .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                           .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
                                       });

        }

        void configureRenderContext(RenderPassManager &rpm) const override {
            rpm.appendAttachment(ATTACHMENT_PRIMARY_COL, {
                                     .flags = 0,
                                     .format = VK_FORMAT_R8G8B8A8_SRGB,
                                     .samples = VK_SAMPLE_COUNT_1_BIT,
                                     .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                     .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                                     .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                     .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                     .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                     .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                 },
                                 contexts.at(PRIMARY_SECONDARY_LINK_FRAME_IMG_CONTEXT));
            rpm.appendAttachment(ATTACHMENT_SECONDARY_COLOR, {
                                     .flags = 0,
                                     .format = VK_FORMAT_R8G8B8A8_SRGB,
                                     .samples = VK_SAMPLE_COUNT_1_BIT,
                                     .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                     .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                                     .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                     .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                     .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                     .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                                 },
                                 ImageContext::fromSwapchain(
                                     swapchain, core.getPhysicalDevice().getMaxFramesInFlight()));


            rpm.appendSubpass(SUBPASS_PRIMARY);
            rpm.appendReference(ATTACHMENT_PRIMARY_COL, RenderPassAttachmentType::COLOR);
            rpm.appendSubpass(SUBPASS_SECONDARY);
            rpm.appendReference(ATTACHMENT_PRIMARY_COL, RenderPassAttachmentType::INPUT);
            rpm.appendReference(ATTACHMENT_SECONDARY_COLOR, RenderPassAttachmentType::COLOR);

            rpm.appendDependency({
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = rpm.getSubpassIndex(SUBPASS_PRIMARY),
                .srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask = 0,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dependencyFlags = 0
            });
            rpm.appendDependency({
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = rpm.getSubpassIndex(SUBPASS_SECONDARY),
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask = 0,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dependencyFlags = 0
            });
            rpm.appendDependency({
                .srcSubpass = rpm.getSubpassIndex(SUBPASS_PRIMARY),
                .dstSubpass = rpm.getSubpassIndex(SUBPASS_SECONDARY),
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
                .dependencyFlags = 0
            });
            rpm.appendDependency({
                .srcSubpass = rpm.getSubpassIndex(SUBPASS_SECONDARY),
                .dstSubpass = VK_SUBPASS_EXTERNAL,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = 0,
                .dependencyFlags = 0
            });
        }
    };
}
