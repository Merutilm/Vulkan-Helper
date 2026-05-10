//
// Created by Merutilm on 2026-02-08.
//

#include "SampleRCC.hpp"

namespace merutilm::vkh_demo {

    void SampleRCC::configure(vkh::RenderPassManager &rpm) {

        rpm.appendAttachment(PRESENT_ATTACHMENT_INDEX, {
            .flags = 0,
            .format = VK_FORMAT_R8G8B8A8_UNORM,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        }, swapchainImageContextGetter());


        rpm.appendSubpass(SUBPASS_INDEX);
        rpm.appendReference(PRESENT_ATTACHMENT_INDEX, vkh::RenderPassAttachmentType::COLOR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        rpm.appendDependency({
            .srcSubpass = SUBPASS_INDEX,
            .dstSubpass = VK_SUBPASS_EXTERNAL,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dstAccessMask = 0,
            .dependencyFlags = 0
        });

    }
} // namespace merutilm
