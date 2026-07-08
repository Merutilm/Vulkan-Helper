//
// Created by Merutilm on 7/7/26.
//

#pragma once
#ifdef VKH_USE_IMGUI
#include "vulkan_helper/engine/graphics/RenderPassGraphGenerator.hpp"
namespace merutilm::vkh {
    struct RCCImGui : RenderPassGraphGenerator {
        static constexpr uint32_t SUBPASS_INDEX = 0;
        static constexpr uint32_t PRESENT_ATTACHMENT_INDEX = 0;

        template<typename F> requires std::is_invocable_r_v<MultiframeImageContext, F>
        explicit RCCImGui(Core & core, SharedImageContext & sharedImageContext, F &&swapchainImageContextGetter): RenderPassGraphGenerator(core, sharedImageContext, std::forward<F>(swapchainImageContextGetter)) {
        }

    protected:
        void configure() override;
    };
}
#endif