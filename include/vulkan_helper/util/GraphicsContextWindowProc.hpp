//
// Created by Merutilm on 2025-08-24.
//

#pragma once
#include <vulkan_helper/core/GraphicsContextWindow.hpp>

namespace merutilm::vkh {
    struct GraphicsContextWindowProc {
        explicit GraphicsContextWindowProc() = delete;



        static LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);


        static LRESULT runListeners(GraphicsContextWindow & window, HWND hwnd, UINT message, WPARAM wparam,
                                    LPARAM lparam);
    };
}
