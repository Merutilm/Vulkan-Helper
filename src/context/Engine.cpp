//
// Created by Merutilm on 2025-07-13.
//

#include "Engine.hpp"

#include "../exception/exception.hpp"

namespace merutilm::vkh {

    Engine::Engine() {
        Engine::attach();
    }

    Engine::~Engine() {
        Engine::detach();
    }

    std::unique_ptr<Engine> Engine::createEngine() {
        return std::make_unique<Engine>();
    }

    float Engine::getTime() const {
        using namespace std::chrono;
        const auto currentTime = high_resolution_clock::now();
        return std::chrono::duration_cast<duration<float>>(currentTime - startTime).count();
    }


    void Engine::attach() {
        instance = std::make_unique<Instance>(true);

        startTime = std::chrono::high_resolution_clock::now();
    }

    void Engine::attachWindow(const WindowCreateInfo &windowCreateInfo) {
        const std::string &attachmentName = windowCreateInfo.windowAttachmentName;
        if (windowContexts.contains(attachmentName)) {
            throw exception_invalid_args("Window already exists");
        }

        auto window = std::make_unique<Window>(windowCreateInfo.windowTitleName.data(), windowCreateInfo.framerate,
                                                windowCreateInfo.width, windowCreateInfo.height);
        auto surface = std::make_unique<Surface>(*instance, *window);

        if (physicalDevice == nullptr) {
            physicalDevice = std::make_unique<PhysicalDevice>(*instance, *surface);
            logicalDevice = std::make_unique<LogicalDevice>(*instance, *physicalDevice);
        }else if (!PhysicalDevice::isDeviceSuitable(physicalDevice->getPhysicalDeviceHandle(), surface->getSurfaceHandle())){
            throw exception_invalid_args("Invalid window provided");
        }

        auto swapchain = std::make_unique<Swapchain>(*surface, *physicalDevice, *logicalDevice);
        windowContexts[attachmentName] = WindowContext{std::move(window), std::move(surface), std::move(swapchain)};

    }


    void Engine::detach() {
        windowContexts.clear();
        logicalDevice = nullptr;
        physicalDevice = nullptr;
        instance = nullptr;
    }
}
