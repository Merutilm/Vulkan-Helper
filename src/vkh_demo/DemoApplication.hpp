//
// Created by Merutilm on 2026-02-03.
//

#pragma once
#include "SampleRenderer.hpp"
#include "vulkan_helper/Application.hpp"

namespace merutilm::vkh_demo {
    struct DemoApplication final : vkh::Application{

        std::optional<SampleRenderer> renderer;


        explicit DemoApplication(const vkh::WindowInitializerSettings& rootWindowInitializerSettings);

        ~DemoApplication() override;

        void refreshSharedImgContexts(VkExtent2D extent) override;

        void createRenderers() override;

        void addListeners() override;

        void update() override;


    protected:
        void init() override;

        void cleanup() override;

#ifdef VKH_USE_IMGUI
        void renderImGui() override;
#endif
    };
}
