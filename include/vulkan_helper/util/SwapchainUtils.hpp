//
// Created by Merutilm on 2025-07-26.
//

#pragma once
#include <vulkan_helper/base/vkh.hpp>

namespace merutilm::vkh {
    struct SwapchainUtils {
        SwapchainUtils() = delete;


        template <typename F> requires std::is_invocable_r_v<void, F, uint32_t>
        static void renderFrame(WindowContextRef wc, uint32_t *frameIndex, F&&renderer);

        static void changeFrameIndex(CoreRef core, uint32_t *frameIndex);

        static uint32_t begin(WindowContextRef wc, const uint32_t frameIndex);

        static void end(WindowContextRef wc, const uint32_t frameIndex, uint32_t swapchainImageIndex);
    };

    template<typename F> requires std::is_invocable_r_v<void, F, unsigned>
    void SwapchainUtils::renderFrame(WindowContextRef wc, uint32_t *frameIndex, F &&renderer) {
        if (wc.getWindow().isUnrenderable()) {
            return;
        }
        changeFrameIndex(wc.core, frameIndex);
        auto swapchainImageIndex = begin(wc, *frameIndex);
        renderer(swapchainImageIndex);
        end(wc, *frameIndex, swapchainImageIndex);
    }

}
