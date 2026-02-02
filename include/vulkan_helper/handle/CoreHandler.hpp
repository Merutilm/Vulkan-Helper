//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include <vulkan_helper/handle/Handler.hpp>
#include <vulkan_helper/core/Core.hpp>

namespace merutilm::vkh {
    struct CoreHandler : public Handler {
        const CoreRef core;

        explicit CoreHandler(const CoreRef core) : core(core) {};

        ~CoreHandler() override = default;

        CoreHandler(const CoreHandler &) = delete;

        CoreHandler &operator=(const CoreHandler &) = delete;

        CoreHandler(CoreHandler &&) = delete;

        CoreHandler &operator=(CoreHandler &&) = delete;

    };
}
