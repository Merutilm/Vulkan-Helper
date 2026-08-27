//
// Created by Merutilm on 2026-02-08.
//

#include "SampleRenderer.hpp"

#include "SampleRCC.hpp"
#include "vulkan_helper/engine/executor/RenderPassFullscreenRecorder.hpp"
#include "vulkan_helper/util/BarrierUtils.hpp"
#include "vulkan_helper/util/RenderContextUtils.hpp"

namespace merutilm::vkh_demo {

    void SampleRenderer::init() {

        rc = vkh::RenderContextUtils::attachRenderContext<SampleRCC>(nullptr, configurators, engine, wc, [this] { return wc.getSwapchain().getSwapchainExtent(); },
                [this] { return vkh::ImageContext::fromSwapchain(wc.core, wc.getSwapchain()); });

        finishPipelineInitialization();
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
        vkh::RenderPassFullscreenRecorder::cmdFullscreenForSwapchainRenderPass(wc, *rc, frameIndex, swapchainImageIndex);

#ifdef VKH_USE_IMGUI
        vkh::BarrierUtils::cmdOverlaySwapchain(
                wc.getCommandBufferGroup().getCommandBufferHandle(frameIndex),
                wc.getSwapchain().getSwapchainImages()[swapchainImageIndex]);

        RendererImGui::cmdRender(swapchainImageIndex);
#endif
    }

} // namespace merutilm::vkh_demo
