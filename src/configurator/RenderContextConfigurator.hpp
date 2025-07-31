//
// Created by Merutilm on 2025-07-22.
//

#pragma once
#include <ranges>

#include "../manage/RenderPassManager.hpp"

namespace merutilm::vkh {
    /**
     * RenderPass Framebuffer Configurator
     */
    struct RenderContextConfigurator {
        const Core &core;
        const Swapchain &swapchain;
        std::unordered_map<std::string, MultiframeImageContext, StringHasher, std::equal_to<>> contexts = {};

        explicit RenderContextConfigurator(const Core &core, const Swapchain &swapchain) : core(core), swapchain(swapchain) {
        }

        virtual ~RenderContextConfigurator() {
            cleanupContexts();
        }

        RenderContextConfigurator(const RenderContextConfigurator &) = delete;

        RenderContextConfigurator &operator=(const RenderContextConfigurator &) = delete;

        RenderContextConfigurator(RenderContextConfigurator &&) = delete;

        RenderContextConfigurator &operator=(RenderContextConfigurator &&) = delete;

        void configure(RenderPassManager &manager) {
            configureImageContext();
            configureRenderContext(manager);
        }

        virtual void configureImageContext() = 0;

        virtual void configureRenderContext(RenderPassManager &manager) const = 0;

        const MultiframeImageContext &getImageContext(const std::string &name) {
            if (!contexts.contains(name)) {
                throw exception_invalid_args("Failed to load image context. given context not exist : " + name);
            }
            return contexts.at(name);
        }

        void cleanupContexts() {
            for (const auto &context : std::ranges::views::values(contexts)) {
                ImageContext::destroy(core, context);
            }
            contexts.clear();
        }

    };
}
