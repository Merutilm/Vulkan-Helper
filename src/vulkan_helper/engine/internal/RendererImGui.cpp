//
// Created by Merutilm on 7/7/26.
//

#ifdef VKH_USE_IMGUI
#include <vulkan_helper/engine/internal/RendererImGui.hpp>

#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "vulkan_helper/engine/executor/RenderPassFullscreenRecorder.hpp"
#include "vulkan_helper/engine/internal/RCCImGui.hpp"
#include "vulkan_helper/util/BarrierUtils.hpp"
namespace merutilm::vkh {


    void RendererImGui::init() {
        rc = wc.attachRenderContext<RCCImGui>(
              [this] { return wc.getSwapchain().getSwapchainExtent(); },
              [this] { return ImageContext::fromSwapchain(wc.core, wc.getSwapchain()); });

    }
    void RendererImGui::cleanup() {

    }
    void RendererImGui::beforeCmdRender() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        renderFunc();
        ImGui::Render();
    }
    void RendererImGui::cmdRender(const uint32_t swapchainImageIndex) {

        ImDrawData* drawData = ImGui::GetDrawData();
        const auto renderPassRecorder = RenderPassFullscreenRecorder(
                wc, *rc, frameIndex, swapchainImageIndex);
        ImGui_ImplVulkan_RenderDrawData(drawData, wc.getCommandBuffer().getCommandBufferHandle(frameIndex));
    }
} // namespace merutilm::vkh
#endif