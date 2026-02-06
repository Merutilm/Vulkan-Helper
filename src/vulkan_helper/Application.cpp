//
// Created by Merutilm on 2026-02-06.
//


#include <vulkan_helper/Application.hpp>

#include "vulkan_helper/util/GraphicsContextWindowProc.hpp"

namespace merutilm::vkh {


    Application::Application(WindowInitializerSettings &&settings) {
        Application::init();
    }

    Application::~Application() { Application::cleanup(); }


    void Application::init() {
    }

    void Application::cleanup() {
    }

} // namespace merutilm::vkh
