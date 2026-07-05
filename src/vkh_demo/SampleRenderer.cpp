//
// Created by Merutilm on 2026-02-08.
//

#include "SampleRenderer.hpp"

#include "SampleRCC.hpp"
#include "vulkan_helper/engine/executor/RenderPassFullscreenRecorder.hpp"

namespace merutilm::vkh_demo {

    void SampleRenderer::init() {
        sample = vkh::PipelineConfigurator::create<GPCSample>(configurators, engine, 0, SampleRCC::CONTEXT_INDEX,
                                                     SampleRCC::SUBPASS_INDEX, vertexBufferPP, indexBufferPP);
    }

    void SampleRenderer::cleanup() {

    }

    void SampleRenderer::beforeCmdRender() {

    }

    void SampleRenderer::cmdRender(uint32_t swapchainImageIndex) {


        vkh::RenderPassFullscreenRecorder::cmdFullscreenPresentOnlyRenderPass<SampleRCC>(wc, frameIndex, swapchainImageIndex, {sample}, {{}});

    }

}