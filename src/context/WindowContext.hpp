//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <memory>

#include "../obj/Window.hpp"
#include "../obj/Surface.hpp"
#include "../obj/Swapchain.hpp"

namespace merutilm::vkh {
    struct WindowContext final {
        std::unique_ptr<Window> window = nullptr;
        std::unique_ptr<Surface> surface = nullptr;
        std::unique_ptr<Swapchain> swapchain = nullptr;
    };
}
