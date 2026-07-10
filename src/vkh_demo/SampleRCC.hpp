//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include <vulkan_helper/engine/graphics/RenderPassGraphGenerator.hpp>

namespace merutilm::vkh_demo {
    struct SampleRCC final : vkh::RenderPassGraphGenerator {

        vkh::RenderPassAttachment * swapchainImageAttachment = nullptr;



        template<typename F> requires std::is_invocable_r_v<vkh::MultiframeImageContext, F>
        SampleRCC(vkh::Engine &engine, vkh::WindowContext &wc, F &&swapchainImageContextGetter) : RenderPassGraphGenerator(
            engine, wc, std::forward<F>(swapchainImageContextGetter)) {}


    protected:
        void configureAttachments() override;

        void configurePipelines() override;
    };
}
