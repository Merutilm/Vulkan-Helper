//
// Created by Merutilm on 2026-02-03.
//

#pragma once
#include "SampleRenderer.hpp"
#include "vulkan_helper/Application.hpp"

namespace merutilm::vkh_demo {
    struct DemoApplication final : vkh::Application{

        SampleRenderer * renderer;


        using Application::Application;

        ~DemoApplication() override = default;

        void refreshSharedImgContexts(VkExtent2D extent) override;

        void registerRenderers() override;

        void addListeners() override;

        void update() override;


    protected:

#ifdef VKH_USE_IMGUI
        void renderImGui() override;
        void onStart() override;
        void onQuit() override;
#endif
    };
}
