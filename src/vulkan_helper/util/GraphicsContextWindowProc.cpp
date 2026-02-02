//
// Created by Merutilm on 2026-02-03.
//

#include <vulkan_helper/util/GraphicsContextWindowProc.hpp>
namespace merutilm::vkh {
    LRESULT GraphicsContextWindowProc::WinProc(const HWND hwnd, const UINT message, const WPARAM wparam,
        const LPARAM lparam) {
        const auto windowPtr = reinterpret_cast<GraphicsContextWindowPtr>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (windowPtr == nullptr) {
            return DefWindowProcW(hwnd, message, wparam, lparam);
        }

        auto &window = *windowPtr;

        if (message == WM_DESTROY) {
            PostQuitMessage(0);
        }

        return runListeners(window, hwnd, message, wparam, lparam);
    }

    LRESULT GraphicsContextWindowProc::runListeners(GraphicsContextWindowRef window, const HWND hwnd,
        const UINT message, const WPARAM wparam, const LPARAM lparam) {
        if (const auto &listeners = window.getListeners();
            listeners.contains(message)) {
            return listeners.at(message)(window, hwnd, wparam, lparam);
        }
        return DefWindowProcW(hwnd, message, wparam, lparam);
    }
}
