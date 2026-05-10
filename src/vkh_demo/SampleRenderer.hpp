//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include "vulkan_helper/engine/graphics/Renderer.hpp"
#include "GPCSample.hpp"

namespace merutilm::vkh_demo {

    struct SampleRenderer final : vkh::Renderer {

        GPCSample * sample = nullptr;

        explicit SampleRenderer(vkh::Engine &engine, vkh::WindowContext &wc, vkh::VertexBuffer &vertexBufferPP, vkh::IndexBuffer &indexBufferPP) : Renderer(engine, wc, vertexBufferPP, indexBufferPP) {
            SampleRenderer::init();
        }

        ~SampleRenderer() override {
            SampleRenderer::cleanup();
        }

        void init() override;
        void cleanup() override;

    private:
        void beforeCmdRender() override;
        void cmdRender(uint32_t swapchainImageIndex) override;
    };
}
