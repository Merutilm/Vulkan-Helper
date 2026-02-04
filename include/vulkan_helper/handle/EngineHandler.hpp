//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include <vulkan_helper/engine/Engine.hpp>
#include <vulkan_helper/handle/Handler.hpp>

namespace merutilm::vkh {
    struct EngineHandler : Handler {
        const Engine & engine;

        explicit EngineHandler(const Engine & engine) : engine(engine) {};

        ~EngineHandler() override = default;

        EngineHandler(const EngineHandler &) = delete;

        EngineHandler &operator=(const EngineHandler &) = delete;

        EngineHandler(EngineHandler &&) = default;

        EngineHandler &operator=(EngineHandler &&) = delete;

    };
}
