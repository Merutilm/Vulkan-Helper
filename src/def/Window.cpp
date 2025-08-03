//
// Created by Merutilm on 2025-07-07.
//

#include "Window.hpp"
#include <chrono>
#include <windows.h>

namespace merutilm::vkh {
    Window::Window(const wchar_t *windowName, const float framerate, const uint32_t width, const uint32_t height) : windowName(windowName), framerate(framerate), width(width), height(height) {
        Window::init();
    }


    Window::~Window() {
        Window::destroy();
    }

    void Window::start() const {
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

            if (auto elapsed = duration_cast<milliseconds>(now - started); static_cast<float>(elapsed.count()) > 1000 / framerate) {
                started = now;
                for (const auto &renderer : renderers) {
                    renderer();
                }
            }
        }
    }


    void Window::init() {
        registerClass();
        createWindow();
    }


    void Window::registerClass() {
        SetProcessDPIAware();
        const WNDCLASSEXW wc = {
            .cbSize = sizeof(WNDCLASSEXW),
            .style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW,
            .lpfnWndProc = winProc,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = GetModuleHandle(nullptr),
            .hIcon = nullptr,
            .hCursor = nullptr,
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = CLASS_NAME,
            .hIconSm = nullptr
        };
        RegisterClassExW(&wc);
    }


    void Window::createWindow() {
        constexpr DWORD style = WS_OVERLAPPEDWINDOW | WS_SYSMENU;
        window = CreateWindowExW(0, CLASS_NAME, windowName, style, 0, 0,
                                 CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        RECT rect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
        AdjustWindowRectEx(&rect, style, false, false);
        SetWindowPos(window, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);

        ShowWindow(window, SW_SHOW);
        UpdateWindow(window);

    }


    LRESULT Window::winProc(const HWND hwnd, const UINT message, const WPARAM wparam, const LPARAM lparam) {
        const auto windowPtr = reinterpret_cast<Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (windowPtr == nullptr) {
            return DefWindowProcW(hwnd, message, wparam, lparam);
        }

        auto &window = *windowPtr;

        if (message == WM_DESTROY) {
            PostQuitMessage(0);
        }

        return window.runListeners(hwnd, message, wparam, lparam);
    }

    LRESULT Window::runListeners(const HWND hwnd, const UINT message, const WPARAM wparam, const LPARAM lparam) {
        if (listeners.contains(message)) {
            return listeners[message](*this);
        }
        return DefWindowProc(hwnd, message, wparam, lparam);
    }

    void Window::destroy() {
        //nothing to do.
    }
}
