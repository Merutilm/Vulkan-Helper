//
// Created by Merutilm on 2026-02-07.
//

#ifdef WIN32
#include <vulkan_helper/window/win/Window.hpp>
#include "vulkan_helper/base/logger.hpp"

namespace merutilm::vkh {
    Window::Window(WindowInitializerSettings &&settings) : PlatformWindowBase(std::move(settings)) { Window::init(); }

    Window::~Window() { Window::cleanup(); }


    void Window::initWindow() {
        const HMENU hMenubar = initializerSettings.useMenubar ? initMenu() : nullptr;
        createMainWindow(hMenubar);
        createRenderWindow();
        setClientSize(initializerSettings.width, initializerSettings.height);
        prepareWindow();
    }
    LRESULT Window::MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        return 0;
    }


    LRESULT Window::RenderWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        return 0;
    }

    HMENU Window::initMenu() {
        const HMENU hMenubar = CreateMenu();
        menubar.emplace(hMenubar);
        return hMenubar;
    }
    void Window::setClientSize(int width, int height) const {
        const RECT rect = {0, 0, width, height};
        RECT adjusted = rect;
        const auto &[top, left, bottom, right] = initializerSettings.paddings;
        AdjustWindowRect(&adjusted, WS_OVERLAPPEDWINDOW | WS_SYSMENU, true);

        SetWindowPos(mainWindow, nullptr, 0, 0, adjusted.right - adjusted.left + left + right,
                     adjusted.bottom - adjusted.top + bottom + top, SWP_NOMOVE | SWP_NOZORDER);
        adjustClient(rect);
    }

    void Window::adjustClient(const RECT &rect) const {
        const auto &paddings = initializerSettings.paddings;
        SetWindowPos(renderWindow, nullptr, paddings.left, paddings.top, rect.right - rect.left, rect.bottom - rect.top,
                     SWP_NOZORDER);
    }

    void Window::resolveWindowResizeEnd() const {
        RECT rect;
        const auto &paddings = initializerSettings.paddings;
        GetClientRect(mainWindow, &rect);
        rect.bottom -= paddings.bottom;
        rect.right -= paddings.right;
        if (rect.bottom - rect.top > 0 || rect.right - rect.left > 0) {
            adjustClient(rect);
        }
    }

    void Window::prepareWindow() const {
        ShowWindow(mainWindow, SW_SHOW);
        UpdateWindow(mainWindow);
        SetWindowLongPtr(mainWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        SetWindowLongPtr(renderWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }
    void Window::createMainWindow(HMENU hMenubar) {
        const WNDCLASSEX wcex = {
                .cbSize = sizeof(WNDCLASSEX),
                .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
                .lpfnWndProc = MainWindowProc,
                .hInstance = GetModuleHandle(nullptr),
                .hIcon = static_cast<HICON>(LoadImage(GetModuleHandleW(nullptr), initializerSettings.icon, IMAGE_ICON,
                                                      32, 32, LR_DEFAULTCOLOR)),
                .hCursor = nullptr,
                .hbrBackground = nullptr,
                .lpszMenuName = nullptr,
                .lpszClassName = MAIN_CLASS_NAME,
        };

        if (!RegisterClassEx(&wcex))
            throw exception_init("Failed to register class : Main Window");

        mainWindow = CreateWindowEx(0, MAIN_CLASS_NAME, initializerSettings.name, WS_OVERLAPPEDWINDOW | WS_SYSMENU,
                                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, hMenubar,
                                    nullptr, nullptr);
        if (!mainWindow) {
            throw exception_init("Failed to create window!\n");
        }
    }
    void Window::createRenderWindow() {
        const WNDCLASSEX wcex = {
                .cbSize = sizeof(WNDCLASSEX),
                .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
                .lpfnWndProc = RenderWindowProc,
                .hInstance = GetModuleHandle(nullptr),
                .hIcon = nullptr,
                .hCursor = nullptr,
                .hbrBackground = nullptr,
                .lpszMenuName = nullptr,
                .lpszClassName = RENDER_CLASS_NAME,
        };
        if (!RegisterClassEx(&wcex))
            throw exception_init("Failed to register class : Render Window");

        renderWindow =
                CreateWindowEx(0, RENDER_CLASS_NAME, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, mainWindow, nullptr, nullptr, nullptr);

        if (!renderWindow) {
            throw exception_init("Failed to create render window!\n");
        }
    }

    void Window::init() {
        SetProcessDPIAware();
        initWindow();
    }

    void Window::cleanup() {}


} // namespace merutilm::vkh
#endif
