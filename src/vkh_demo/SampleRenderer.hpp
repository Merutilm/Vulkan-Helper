//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include "GPCSample.hpp"
#include "vulkan_helper/engine/graphics/Renderer.hpp"

#ifdef VKH_USE_IMGUI
#include "vulkan_helper/engine/internal/RendererImGui.hpp"
using SampleRendererBase = merutilm::vkh::RendererImGui;

#else
using SampleRendererBase = merutilm::vkh::Renderer;
#endif

namespace merutilm::vkh_demo {

    struct SampleRenderer final : SampleRendererBase {
        vkh::RenderContext *rc = nullptr;

#ifdef VKH_USE_IMGUI
        template<typename F> requires std::is_invocable_r_v<void, F>
        explicit SampleRenderer(vkh::Engine &engine, vkh::WindowContext &wc, F&& imguiFunc) : RendererImGui(engine, wc, imguiFunc) {
            SampleRenderer::init();
        }
#else
        explicit SampleRenderer(vkh::Engine &engine, vkh::WindowContext &wc) : Renderer(engine, wc) {
            SampleRenderer::init();
        }
#endif

        ~SampleRenderer() override {
            SampleRenderer::cleanup();
        }

    protected:
        void init() override;
        void cleanup() override;
        void beforeCmdRender() override;
        void cmdRender(uint32_t swapchainImageIndex) override;

    };
}
