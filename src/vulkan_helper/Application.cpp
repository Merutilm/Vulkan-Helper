//
// Created by Merutilm on 2026-02-06.
//


#include <vulkan_helper/Application.hpp>


namespace merutilm::vkh {

    Application::Application() {
        Application::init();
    }

    Application::~Application() { Application::cleanup(); }


    void Application::init() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        if (!glfwVulkanSupported()) {
            throw exception_init("Vulkan is not supported!");
        }
        engine.emplace();
    }

    void Application::cleanup() {
        sharedResource.reset();
        engine.reset();
        glfwTerminate();
    }

} // namespace merutilm::vkh
