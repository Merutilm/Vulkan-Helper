//
// Created by Merutilm on 2025-07-22.
//

#pragma once
#include "RenderPassConfigurator.hpp"

namespace merutilm::vkh {
    struct BasicRenderPassConfigurator final : RenderPassConfigurator {

        inline static const std::string SUBPASS_PRIMARY = "Subpass";
        inline static const std::string ATTACHMENT_COLOR = "color";
        inline static const std::string ATTACHMENT_DEPTH = "depth_stencil";

        void configure(RenderPassManager &rpm) const override {
            rpm.appendSubpass(SUBPASS_PRIMARY);
            rpm.appendAttachment(ATTACHMENT_COLOR, RenderPassAttachmentType::COLOR, {
                                     .flags = 0,
                                     .format = VK_FORMAT_R8G8B8A8_SRGB,
                                     .samples = VK_SAMPLE_COUNT_1_BIT,
                                     .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                     .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                                     .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                     .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                     .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                     .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                                 });
            rpm.appendAttachment(ATTACHMENT_DEPTH, RenderPassAttachmentType::DEPTH_STENCIL, {
                                     .flags = 0,
                                     .format = VK_FORMAT_D32_SFLOAT,
                                     .samples = VK_SAMPLE_COUNT_1_BIT,
                                     .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                     .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                     .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                     .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                     .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                     .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                                 }, ImageInitInfo{
                                     .imageType = VK_IMAGE_TYPE_2D,
                                     .imageViewType = VK_IMAGE_VIEW_TYPE_2D,
                                     .imageFormat = VK_FORMAT_D32_SFLOAT,
                                     .extent = {},
                                     .mipLevels = 1,
                                     .arrayLayers = 1,
                                     .samples = VK_SAMPLE_COUNT_1_BIT,
                                     .imageTiling = VK_IMAGE_TILING_OPTIMAL,
                                     .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                     .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                     .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                 });
            rpm.appendDependency({
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = rpm.getSubpassIndex(SUBPASS_PRIMARY),
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask = 0,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dependencyFlags = 0
            });
            rpm.appendDependency({
                .srcSubpass = rpm.getSubpassIndex(SUBPASS_PRIMARY),
                .dstSubpass = VK_SUBPASS_EXTERNAL,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = 0,
                .dependencyFlags = 0
            });
        }
    };
}
