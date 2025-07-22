//
// Created by Merutilm on 2025-07-22.
//

#pragma once
#include "../manage/RenderPassManager.hpp"

namespace merutilm::vkh {
    struct RenderPassConfigurator {
        RenderPassConfigurator() = default;

        virtual ~RenderPassConfigurator() = default;

        RenderPassConfigurator(const RenderPassConfigurator &) = delete;

        RenderPassConfigurator &operator=(const RenderPassConfigurator &) = delete;

        RenderPassConfigurator(RenderPassConfigurator &&) = delete;

        RenderPassConfigurator &operator=(RenderPassConfigurator &&) = delete;

        virtual void configure(RenderPassManager &manager) const = 0;
    };
}
