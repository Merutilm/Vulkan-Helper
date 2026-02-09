//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include <vulkan_helper/engine/configurator/RenderContextConfigurator.hpp>

namespace merutilm {
    struct SampleRCC final : vkh::RenderContextConfigurator {
        static constexpr uint32_t CONTEXT_INDEX = 0;
        static constexpr uint32_t SUBPASS_INDEX = 0;
        static constexpr uint32_t PRESENT_ATTACHMENT_INDEX = 0;

        template<typename F> requires std::is_invocable_r_v<vkh::MultiframeImageContext, F>
        SampleRCC(vkh::Core &core, vkh::SharedImageContext &sharedImageContext, const F &swapchainImageContextGetter) : RenderContextConfigurator(
            core, sharedImageContext, swapchainImageContextGetter) {}



        void configure(vkh::RenderPassManager &rpm) override;
    };
}
