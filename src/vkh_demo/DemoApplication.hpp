//
// Created by Merutilm on 2026-02-03.
//

#pragma once
#include "SampleRenderer.hpp"
#include "vulkan_helper/Application.hpp"

namespace merutilm {
    struct DemoApplication final : vkh::Application{

        vkh::WindowContext * wc = nullptr;
        std::optional<SampleRenderer> renderer;

        DemoApplication();

        void init() override;

        void createWindow();

        void addListeners();
        void createContexts() const;
        void createRenderers();
        void update();

        void start() const;

    };
}
