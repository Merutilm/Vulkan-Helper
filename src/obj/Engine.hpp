//
// Created by Merutilm on 2025-07-13.
//

#pragma once
#include <chrono>
#include <memory>

#include "Instance.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"
#include "Window.hpp"
#include "../struct/EngineInitInfo.hpp"

namespace merutilm::vkh {
    class Engine final : public Handler {
        const EngineInitInfo engineInitInfo;

        std::unique_ptr<Window> window = nullptr;

        std::unique_ptr<Instance> instance = nullptr;
        std::unique_ptr<Surface> surface = nullptr;
        std::unique_ptr<PhysicalDevice> physicalDevice = nullptr;
        std::unique_ptr<LogicalDevice> logicalDevice = nullptr;
        std::unique_ptr<Swapchain> swapchain = nullptr;

        std::chrono::high_resolution_clock::time_point startTime;

    public:

        explicit Engine(EngineInitInfo &&engineInitInfo);

        ~Engine() override;

        Engine(const Engine &) = delete;

        Engine &operator=(const Engine &) = delete;

        Engine(Engine &&) = delete;

        Engine &operator=(Engine &&) = delete;

        [[nodiscard]] const Window &getWindow() const { return *window; }

        [[nodiscard]] const Instance &getInstance() const { return *instance; }

        [[nodiscard]] const Surface &getSurface() const { return *surface; }

        [[nodiscard]] const PhysicalDevice &getPhysicalDevice() const { return *physicalDevice; }

        [[nodiscard]] const LogicalDevice &getLogicalDevice() const { return *logicalDevice; }

        [[nodiscard]] const Swapchain &getSwapchain() const { return *swapchain; }

        [[nodiscard]] Window &getWindow() { return *window; }

        [[nodiscard]] Instance &getInstance() { return *instance; }

        [[nodiscard]] Surface &getSurface() { return *surface; }

        [[nodiscard]] PhysicalDevice &getPhysicalDevice() { return *physicalDevice; }

        [[nodiscard]] LogicalDevice &getLogicalDevice() { return *logicalDevice; }

        [[nodiscard]] Swapchain &getSwapchain() { return *swapchain; }

        [[nodiscard]] static std::unique_ptr<Engine> createEngine(EngineInitInfo &&engineInfo);

        [[nodiscard]] float getTime() const;

    private:
        void attach() override;

        void detach() override;
    };
}
