//
// Created by Merutilm on 2025-07-07.
//

#include "Application.hpp"

namespace merutilm::vkh {
    Application::Application() {
        Application::init();
    }

    Application::~Application() {
        Application::destroy();
    }


    void Application::start() const {
        engine->getWindow().start();
    }

    void Application::init() {
        EngineInitInfo engineInitInfo = {
            .windowName = L"Main",
            .width = 1280,
            .height = 720,
            .framerate = 120
        };
        engine = Engine::createEngine(std::move(engineInitInfo));
        commandPool = std::make_unique<CommandPool>(*engine);
        commandBuffer = std::make_unique<CommandBuffer>(*engine, *commandPool);
        // renderPass = std::make_unique<RenderPass>(*engine, );
    }

    void Application::destroy() {
        renderPass = nullptr;
        commandBuffer = nullptr;
        commandPool = nullptr;
        engine = nullptr;
    }
}
