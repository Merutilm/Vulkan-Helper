//
// Created by Merutilm on 2026-02-08.
//

#include "SampleRenderer.hpp"

#include "SampleRCC.hpp"
#include "vulkan_helper/engine/executor/RenderPassFullscreenRecorder.hpp"
#include "vulkan_helper/util/BarrierUtils.hpp"

namespace merutilm::vkh_demo {

    void SampleRenderer::init() {

        rc = wc.attachRenderContext<SampleRCC>(engine, [this] { return wc.getSwapchain().getSwapchainExtent(); },
                [this] { return vkh::ImageContext::fromSwapchain(wc.core, wc.getSwapchain()); });

    }

    void SampleRenderer::cleanup() {
        // noop
    }
    void SampleRenderer::beforeCmdRender() {
#ifdef VKH_USE_IMGUI
        RendererImGui::beforeCmdRender();
#endif
    }


    void SampleRenderer::cmdRender(const uint32_t swapchainImageIndex) {
        vkh::RenderPassFullscreenRecorder::cmdFullscreenPresentOnlyRenderPass(wc, *rc, frameIndex, swapchainImageIndex,
                                                                              {sample}, {{}});

#ifdef VKH_USE_IMGUI
        vkh::BarrierUtils::cmdOverlaySwapchain(
                wc.getCommandBuffer().getCommandBufferHandle(frameIndex),
                wc.getSwapchain().getSwapchainImages()[swapchainImageIndex]);

        RendererImGui::cmdRender(swapchainImageIndex);
#endif
    }

} // namespace merutilm::vkh_demo
