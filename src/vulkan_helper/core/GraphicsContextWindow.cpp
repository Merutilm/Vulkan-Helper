//
// Created by Merutilm on 2025-07-07.
//

#include <vulkan_helper/core/GraphicsContextWindow.hpp>
#include <chrono>
#include <windows.h>

namespace merutilm::vkh {
    GraphicsContextWindow::GraphicsContextWindow(const HWND window) : window(window) {
    }

    void GraphicsContextWindow::renderOnce() const {
        for (const auto &renderer: renderers) {
            renderer();
        }
    }

    void GraphicsContextWindow::start() const {
        MSG message;
        using namespace std::chrono;
        auto started = high_resolution_clock::now();

        while (true) {
            if (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessageW(&message);

                if (message.message == WM_QUIT) {
                    break;
                }
            }

            auto now = high_resolution_clock::now();

            if (auto elapsed = duration_cast<milliseconds>(now - started);
                static_cast<float>(elapsed.count()) > 1000 / framerate) {
                started = now;
                for (const auto &renderer: renderers) {
                    renderer();
                }
            }
        }
    }
}
