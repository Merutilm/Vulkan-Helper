//
// Created by Merutilm on 2025-07-09.
//

#pragma once
#include "GraphicsContextWindow.hpp"
#include "Instance.hpp"
#include <vulkan_helper/handle/Handler.hpp>

namespace merutilm::vkh {
    class Surface final : public Handler {

        Instance & instance;
        GraphicsContextWindow & window;

        VkSurfaceKHR surface = nullptr;

    public:
        explicit Surface(Instance & instance, GraphicsContextWindow & window);

        ~Surface() override;

        Surface(const Surface &) = delete;

        Surface &operator=(const Surface &) = delete;

        Surface(Surface &&) noexcept = default;

        Surface &operator=(Surface &&) = delete;

        [[nodiscard]] GraphicsContextWindow & getTargetWindow() const { return window; }

        [[nodiscard]] VkSurfaceKHR getSurfaceHandle() const { return surface; }

    private:
        void init() override;

        void cleanup() override;
    };


}
