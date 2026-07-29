//
// Created by Merutilm on 2026-02-08.
//

#include "SampleRCC.hpp"

#include "GPCSample.hpp"

namespace merutilm::vkh_demo {

    void SampleRCC::configureAttachments() {
        swapchainImageAttachment =
                &appendAttachment({.flags = 0,
                                   .format = engine.getCore().getPhysicalDeviceLoader().getPrimarySurfaceFormat(),
                                   .samples = VK_SAMPLE_COUNT_1_BIT,
                                   .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                   .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                                   .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                   .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                   .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
#ifdef VKH_USE_IMGUI
                                   .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
#else
                                   .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
#endif
                                  },
                                  swapchainImageContextGetter());
    }
    void SampleRCC::configurePipelines() {

        registerPipeline<GPCSample>(nullptr, {},
                                {swapchainImageAttachment,
                                 vkh::RenderPassAttachmentReference{vkh::RenderPassAttachmentType::COLOR,
                                                                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL},
                                 std::nullopt, std::nullopt},
                                [] { return vkh::DescIndexPicker{}; });
    }
} // namespace merutilm::vkh_demo
