//
// Created by Merutilm on 2026-02-06.
//


#include <vulkan_helper/Application.hpp>


namespace merutilm::mvk {

    Application::Application() {
        Application::init();
    }

    Application::~Application() { Application::cleanup(); }


    void Application::init() {
    }

    void Application::cleanup() {

    }

} // namespace merutilm::mvk
