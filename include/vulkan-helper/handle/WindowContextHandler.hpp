//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include <vulkan-helper/engine/Engine.hpp>
#include <vulkan-helper/handle/Handler.hpp>

namespace merutilm::vkh {
    struct WindowContextHandler : Handler {
        WindowContext & wc;

        explicit WindowContextHandler(WindowContext & wc) : wc(wc) {};

        ~WindowContextHandler() override = default;

        WindowContextHandler(const WindowContextHandler &) = delete;

        WindowContextHandler &operator=(const WindowContextHandler &) = delete;

        WindowContextHandler(WindowContextHandler &&) = delete;

        WindowContextHandler &operator=(WindowContextHandler &&) = delete;

    };
}
