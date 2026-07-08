//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include <vulkan_helper/engine/graphics/RenderPassGraphGenerator.hpp>

namespace merutilm::vkh_demo {
    struct SampleRCC final : vkh::RenderPassGraphGenerator {
        static constexpr uint32_t SUBPASS_INDEX = 0;
        static constexpr uint32_t COLOR_ATTACHMENT_INDEX = 0;

        template<typename F> requires std::is_invocable_r_v<vkh::MultiframeImageContext, F>
        SampleRCC(vkh::Engine &engine, vkh::WindowContext &wc, F &&swapchainImageContextGetter) : RenderPassGraphGenerator(
            engine, wc, std::forward<F>(swapchainImageContextGetter)) {}


    protected:
        void configure() override;
    };
}
