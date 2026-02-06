//
// Created by Merutilm on 2026-02-07.
//

#pragma once
#include "EventSystem.hpp"
#include "WindowInitializerSettings.hpp"
#include "vulkan_helper/handle/Handler.hpp"

namespace merutilm::vkh {
    struct PlatformWindowBase : Handler {
        EventSystem windowEvent = {};
        WindowInitializerSettings initializerSettings;

        explicit PlatformWindowBase(WindowInitializerSettings &&settings);

        ~PlatformWindowBase() override;
        PlatformWindowBase(const PlatformWindowBase &) = delete;
        PlatformWindowBase &operator=(const PlatformWindowBase &) = delete;
        PlatformWindowBase(PlatformWindowBase &&) noexcept = default;
        PlatformWindowBase & operator=(PlatformWindowBase &&) = delete;

        void init() override;

        void start();

        void cleanup() override;
    };
}