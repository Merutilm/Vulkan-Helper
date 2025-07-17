//
// Created by Merutilm on 2025-07-09.
//

#pragma once
#include <vulkan/vulkan_core.h>

#include "Instance.hpp"
#include "Window.hpp"
#include "../handle/Handler.hpp"

namespace merutilm::vkh {
    class Surface final : public Handler {

        const Instance &instance;
        const Window &window;

        VkSurfaceKHR surface = nullptr;

    public:
        explicit Surface(const Instance &instance, const Window &window);

        ~Surface() override;

        Surface(const Surface &) = delete;

        Surface &operator=(const Surface &) = delete;

        Surface(Surface &&) = delete;

        Surface &operator=(Surface &&) = delete;

        [[nodiscard]] const Window &getTargetWindow() const { return window; }

        [[nodiscard]] VkSurfaceKHR getSurfaceHandle() const { return surface; }

    private:
        void attach() override;

        void detach() override;
    };
}
