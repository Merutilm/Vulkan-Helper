//
// Created by Merutilm on 2025-08-24.
//

#pragma once
#include <vulkan_helper/core/GraphicsContextWindow.hpp>

namespace merutilm::vkh {
    struct GraphicsContextWindowProc {
        explicit GraphicsContextWindowProc() = delete;



        static LRESULT CALLBACK WinProc(const HWND hwnd, const UINT message, const WPARAM wparam,
                                                             const LPARAM lparam);


        static LRESULT runListeners(GraphicsContextWindowRef window, const HWND hwnd, const UINT message, const WPARAM wparam,
                                    const LPARAM lparam);
    };
}
