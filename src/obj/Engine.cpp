//
// Created by Merutilm on 2025-07-13.
//

#include "Engine.hpp"

namespace merutilm::vkh {

    Engine::Engine(EngineInitInfo &&engineInitInfo) : engineInitInfo(std::move(engineInitInfo)) {
        Engine::init();
    }

    Engine::~Engine() {
        Engine::destroy();
    }

    std::unique_ptr<Engine> Engine::createEngine(EngineInitInfo &&engineInfo) {
        return std::make_unique<Engine>(std::move(engineInfo));
    }

    float Engine::getTime() const {
        using namespace std::chrono;
        const auto currentTime = high_resolution_clock::now();
        return std::chrono::duration_cast<duration<float>>(currentTime - startTime).count();
    }


    void Engine::init() {
        instance = std::make_unique<Instance>(true);
        window = std::make_unique<Window>(engineInitInfo.windowName.data(), engineInitInfo.framerate,
                                          engineInitInfo.width, engineInitInfo.height);
        surface = std::make_unique<Surface>(*instance, *window);
        physicalDevice = std::make_unique<PhysicalDevice>(*instance, *surface);
        logicalDevice = std::make_unique<LogicalDevice>(*instance, *physicalDevice);
        swapchain = std::make_unique<Swapchain>(*surface, *physicalDevice, *logicalDevice);
        startTime = std::chrono::high_resolution_clock::now();
    }


    void Engine::destroy() {
        swapchain = nullptr;
        logicalDevice = nullptr;
        physicalDevice = nullptr;
        surface = nullptr;
        window = nullptr;
        instance = nullptr;
    }
}
