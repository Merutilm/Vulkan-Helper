//
// Created by Merutilm on 2026-02-08.
//

#pragma once
#include <vulkan_helper/engine/configurator/RenderContextConfigurator.hpp>

namespace merutilm::mvk_demo {
    struct SampleRCC final : mvk::RenderContextConfigurator {
        static constexpr uint32_t CONTEXT_INDEX = 0;
        static constexpr uint32_t SUBPASS_INDEX = 0;
        static constexpr uint32_t PRESENT_ATTACHMENT_INDEX = 0;

        template<typename F> requires std::is_invocable_r_v<mvk::MultiframeImageContext, F>
        SampleRCC(mvk::Core &core, mvk::SharedImageContext &sharedImageContext, const F &swapchainImageContextGetter) : RenderContextConfigurator(
            core, sharedImageContext, swapchainImageContextGetter) {}



        void configure(mvk::RenderPassManager &rpm) override;
    };
}
