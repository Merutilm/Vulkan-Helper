//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include <vulkan-helper/engine/Engine.hpp>
#include <vulkan-helper/handle/Handler.hpp>

namespace merutilm::vkh {
    struct EngineHandler : Handler {
        Engine & engine;

        explicit EngineHandler(Engine & engine) : engine(engine) {};

        ~EngineHandler() override = default;

        EngineHandler(EngineHandler &) = delete;

        EngineHandler &operator=(EngineHandler &) = delete;

        EngineHandler(EngineHandler &&) = default;

        EngineHandler &operator=(EngineHandler &&) = delete;

    };
}
