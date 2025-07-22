//
// Created by Merutilm on 2025-07-13.
//

#pragma once
#include <chrono>
#include <memory>

#include "../context/WindowContext.hpp"
#include "../exception/exception.hpp"
#include "Instance.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "../struct/StringHasher.hpp"
#include "../struct/WindowCreateInfo.hpp"

namespace merutilm::vkh {
    class Core final : public Handler {
        std::unique_ptr<Instance> instance = nullptr;
        std::unique_ptr<PhysicalDevice> physicalDevice = nullptr;
        std::unique_ptr<LogicalDevice> logicalDevice = nullptr;

        std::unordered_map<std::string, WindowContext, StringHasher, std::equal_to<> > windowContexts =
                {};

        std::chrono::high_resolution_clock::time_point startTime;

    public:

        explicit Core();

        ~Core() override;

        Core(const Core &) = delete;

        Core &operator=(const Core &) = delete;

        Core(Core &&) = delete;

        Core &operator=(Core &&) = delete;

        void createWindow(const WindowCreateInfo &windowCreateInfo);

        [[nodiscard]] const Instance &getInstance() const { return *instance; }

        [[nodiscard]] const PhysicalDevice &getPhysicalDevice() const { return *physicalDevice; }

        [[nodiscard]] const LogicalDevice &getLogicalDevice() const { return *logicalDevice; }

        [[nodiscard]] const WindowContext &getWindowContext(const std::string &windowName) const {
            if (!windowContexts.contains(windowName)) {
                throw exception_invalid_args("Not found window name : " + windowName);
            }
            return windowContexts.at(windowName);
        }

        [[nodiscard]] Instance &getInstance() { return *instance; }

        [[nodiscard]] PhysicalDevice &getPhysicalDevice() { return *physicalDevice; }

        [[nodiscard]] LogicalDevice &getLogicalDevice() { return *logicalDevice; }

        [[nodiscard]] WindowContext &getWindowContext(const std::string &windowName) { return windowContexts[windowName]; }

        [[nodiscard]] static std::unique_ptr<Core> createCore();

        [[nodiscard]] float getTime() const;

    private:
        void attach() override;

        void detach() override;
    };
}
