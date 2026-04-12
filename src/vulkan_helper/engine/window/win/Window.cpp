//
// Created by Merutilm on 2026-02-07.
//

#ifdef WIN32
#include <vulkan_helper/base/logger.hpp>
#include <vulkan_helper/engine/window/win/Window.hpp>

#include "vulkan_helper/base/allocator.hpp"

namespace merutilm::mvk {
    Window::Window(WindowInitializerSettings &&settings) : PlatformWindowBase(std::move(settings)) { Window::init(); }

    Window::~Window() { Window::cleanup(); }


    void Window::initWindow() {
        const HMENU hMenubar = initializerSettings.useMenubar ? initMenu() : nullptr;
        generateKeyMapper();
        createMainWindow(hMenubar);
        createRenderWindow();
        setClientSize(initializerSettings.width, initializerSettings.height);
        prepareWindow();
    }
    LRESULT Window::MainWindowProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {

        const auto windowPtr = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (windowPtr == nullptr) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        auto &window = *windowPtr;


        switch (message) {

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN: {
                if (lParam & (1 << 30)) // is not first input
                    return 0;


                if (wParam == VK_CONTROL)
                    window.controlKeyPressed = true;
                if (wParam == VK_MENU)
                    window.altKeyPressed = true;
                if (wParam == VK_SHIFT)
                    window.shiftKeyPressed = true;

                const auto key = window.findEquivalentKey(wParam);
                window.pressedKeys.emplace(key);
                window.eventSystem.keyboard.onKeyDown.invoke(key);
                return 0;
            }

            case WM_KEYUP:
            case WM_SYSKEYUP: {
                if (wParam == VK_CONTROL)
                    window.controlKeyPressed = false;
                if (wParam == VK_MENU)
                    window.altKeyPressed = false;
                if (wParam == VK_SHIFT)
                    window.shiftKeyPressed = false;
                const auto key = window.findEquivalentKey(wParam);
                window.pressedKeys.erase(key);
                window.eventSystem.keyboard.onKeyUp.invoke(key);
                return 0;
            }
            case WM_KILLFOCUS: {
                window.controlKeyPressed = false;
                window.altKeyPressed = false;
                window.shiftKeyPressed = false;
                window.mouseHovered = false;
                window.leftMouse.reset();
                window.middleMouse.reset();
                window.rightMouse.reset();
                window.pressedKeys.clear();
                window.eventSystem.focus.onFocusLost.invoke();
                return 0;
            }
            case WM_SETFOCUS: {
                window.eventSystem.focus.onFocusGained.invoke();
                return 0;
            }
            case WM_MOUSEWHEEL: {
                window.eventSystem.mouseWheel.onMouseScroll.invoke(GET_WHEEL_DELTA_WPARAM(wParam) /
                                                                   static_cast<float>(WHEEL_DELTA));
                return 0;
            }
            case WM_DESTROY: {
                window.eventSystem.applicationLifecycle.onQuit.invoke();
                PostQuitMessage(0);
                return 0;
            }
            case WM_SIZE: {
                const auto [width, height] = window.calculateClientSizeFromMainWindow();

                if (wParam != SIZE_MAXIMIZED && wParam != SIZE_MINIMIZED && wParam != SIZE_RESTORED) {
                    window.eventSystem.resize.onResizing.invoke(width, height);
                    return 0;
                }

                window.setRenderWindowSize(width, height);
                window.isResizing = false;
                window.eventSystem.resize.onResizeEnd.invoke(width, height);

                if (wParam == SIZE_MINIMIZED) {
                    window.eventSystem.window.onMinimize.invoke();
                } else if (wParam == SIZE_MAXIMIZED){
                    window.eventSystem.window.onMaximize.invoke();
                }else {
                    window.eventSystem.window.onRestore.invoke();
                }
                return 0;
            }

            case WM_ENTERSIZEMOVE: {
                window.isResizing = true;
                window.eventSystem.resize.onResizeBegin.invoke();
                return 0;
            }
            case WM_EXITSIZEMOVE: {
                const auto [width, height] = window.calculateClientSizeFromMainWindow();
                window.setRenderWindowSize(width, height);
                window.isResizing = false;
                window.eventSystem.resize.onResizeEnd.invoke(width, height);
                return 0;
            }
            case WM_DROPFILES: {
                const auto hDrop = reinterpret_cast<HDROP>(wParam);
                const uint32_t fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
                std::vector<std::string> files;
                for (uint32_t i = 0; i < fileCount; ++i) {
                    std::wstring filePath(MAX_PATH, 0);
                    DragQueryFile(hDrop, i, filePath.data(), MAX_PATH);
                    files.emplace_back(ws2s(filePath));
                }

                window.eventSystem.dnd.onFileDrop.invoke(files);
                DragFinish(hDrop);
                return 0;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    LRESULT Window::RenderWindowProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
        const auto windowPtr = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (windowPtr == nullptr) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        auto &window = *windowPtr;

        switch (message) {
            case WM_MOUSELEAVE: {
                window.eventSystem.mouse.onMouseExit.invoke();
                window.mouseHovered = false;
                return 0;
            }
            case WM_LBUTTONDOWN: {
                const int xPos = GET_X_LPARAM(lParam);
                const int yPos = GET_Y_LPARAM(lParam);
                window.leftMouse.isPressed = true;
                window.leftMouse.pressedX = xPos;
                window.leftMouse.pressedY = yPos;
                window.eventSystem.mouse.onMouseDown.invoke(KeyInput::MouseButton::LEFT, xPos, yPos);
                return 0;
            }
            case WM_MBUTTONDOWN: {
                const int xPos = GET_X_LPARAM(lParam);
                const int yPos = GET_Y_LPARAM(lParam);
                window.middleMouse.isPressed = true;
                window.middleMouse.pressedX = xPos;
                window.middleMouse.pressedY = yPos;
                window.eventSystem.mouse.onMouseDown.invoke(KeyInput::MouseButton::MIDDLE, xPos, yPos);
                return 0;
            }
            case WM_RBUTTONDOWN: {
                const int xPos = GET_X_LPARAM(lParam);
                const int yPos = GET_Y_LPARAM(lParam);
                window.rightMouse.isPressed = true;
                window.rightMouse.pressedX = xPos;
                window.rightMouse.pressedY = yPos;
                window.eventSystem.mouse.onMouseDown.invoke(KeyInput::MouseButton::RIGHT, xPos, yPos);
                return 0;
            }
            case WM_MOUSEMOVE: {
                const int xPos = GET_X_LPARAM(lParam);
                const int yPos = GET_Y_LPARAM(lParam);

                if (!window.mouseHovered) {
                    TRACKMOUSEEVENT tme{};
                    tme.cbSize = sizeof(tme);
                    tme.dwFlags = TME_LEAVE;
                    tme.hwndTrack = hWnd;
                    TrackMouseEvent(&tme);
                    window.eventSystem.mouse.onMouseEnter.invoke();
                    window.mouseHovered = true;
                }

                window.eventSystem.mouse.onMouseMove.invoke(xPos, yPos);

                auto &mouseDragEvent = window.eventSystem.mouseDrag;

                auto dragFunc = [xPos, yPos, &mouseDragEvent](MouseState &mouseState,
                                                              const KeyInput::MouseButton mouseButton) {
                    if (!mouseState.isPressed)
                        return;

                    const float dx = static_cast<float>(mouseState.pressedX) - xPos;
                    const float dy = static_cast<float>(mouseState.pressedY) - yPos;

                    if (!mouseState.isDragging && dx * dx + dy * dy > MOUSE_DRAG_THRESHOLD) {
                        mouseDragEvent.onMouseDragStart.invoke(mouseButton, xPos, yPos);
                        mouseState.isDragging = true;
                    }
                    if (mouseState.isDragging)
                        mouseDragEvent.onMouseDrag.invoke(mouseButton, xPos, yPos);
                };

                dragFunc(window.leftMouse, KeyInput::MouseButton::LEFT);
                dragFunc(window.middleMouse, KeyInput::MouseButton::MIDDLE);
                dragFunc(window.rightMouse, KeyInput::MouseButton::RIGHT);

                return 0;
            }

            case WM_LBUTTONUP: {
                const int xPos = GET_X_LPARAM(lParam);
                const int yPos = GET_Y_LPARAM(lParam);
                if (window.leftMouse.isDragging)
                    window.eventSystem.mouseDrag.onMouseDragEnd.invoke(KeyInput::MouseButton::LEFT, xPos, yPos);
                window.eventSystem.mouse.onMouseUp.invoke(KeyInput::MouseButton::LEFT, xPos, yPos);
                window.leftMouse.reset();
                return 0;
            }

            case WM_MBUTTONUP: {
                const int xPos = GET_X_LPARAM(lParam);
                const int yPos = GET_Y_LPARAM(lParam);
                if (window.middleMouse.isDragging)
                    window.eventSystem.mouseDrag.onMouseDragEnd.invoke(KeyInput::MouseButton::MIDDLE, xPos, yPos);
                window.eventSystem.mouse.onMouseUp.invoke(KeyInput::MouseButton::MIDDLE, xPos, yPos);
                window.middleMouse.reset();
                return 0;
            }

            case WM_RBUTTONUP: {
                const int xPos = GET_X_LPARAM(lParam);
                const int yPos = GET_Y_LPARAM(lParam);
                if (window.rightMouse.isDragging)
                    window.eventSystem.mouseDrag.onMouseDragEnd.invoke(KeyInput::MouseButton::RIGHT, xPos, yPos);
                window.eventSystem.mouse.onMouseUp.invoke(KeyInput::MouseButton::RIGHT, xPos, yPos);
                window.rightMouse.reset();
                return 0;
            }

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    std::string Window::ws2s(const std::wstring &str) {
        if (str.empty())
            return {};
        const int sizeRequired =
                WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);
        std::string result(sizeRequired + 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeRequired, nullptr,
                            nullptr);
        return result;
    }
    std::wstring Window::s2ws(const std::string &str) {
        if (str.empty())
            return {};
        const int sizeRequired = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
        std::wstring result(sizeRequired + 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeRequired);
        return result;
    }


    void Window::generateKeyMapper() {
        using namespace KeyInput;

        for (int i = VK_F1; i <= VK_F24; ++i) {
            keyMapper[i] = static_cast<Key>(static_cast<int>(KeyCategory::FUNCTION) + i - VK_F1);
        }
        for (int i = '0'; i <= '9'; ++i) {
            keyMapper[i] = static_cast<Key>(static_cast<int>(KeyCategory::NUM) + i - '0');
        }
        for (int i = VK_NUMPAD0; i <= VK_NUMPAD9; ++i) {
            keyMapper[i] = static_cast<Key>(static_cast<int>(KeyCategory::NUMPAD) + i - VK_NUMPAD0);
        }

        keyMapper[VK_MULTIPLY] = Key::KEY_NUMPAD_MULTIPLY;
        keyMapper[VK_ADD] = Key::KEY_NUMPAD_ADD;
        keyMapper[VK_SEPARATOR] = Key::KEY_NUMPAD_SEPARATOR;
        keyMapper[VK_SUBTRACT] = Key::KEY_NUMPAD_SUBTRACT;
        keyMapper[VK_DECIMAL] = Key::KEY_NUMPAD_DECIMAL;
        keyMapper[VK_DIVIDE] = Key::KEY_NUMPAD_DIVIDE;

        keyMapper[VK_OEM_3] = Key::KEY_BACKQUOTE;
        keyMapper[VK_OEM_MINUS] = Key::KEY_MINUS_UNDERLINE;
        keyMapper[VK_OEM_PLUS] = Key::KEY_PLUS_EQUAL;
        keyMapper[VK_OEM_4] = Key::KEY_LEFTBRACKET;
        keyMapper[VK_OEM_6] = Key::KEY_RIGHTBRACKET;
        keyMapper[VK_OEM_5] = Key::KEY_BACKSLASH; // ansi backslash
        keyMapper[VK_OEM_102] = Key::KEY_BACKSLASH; // iso backslash
        keyMapper[VK_OEM_1] = Key::KEY_SEMICOLON_COLON;
        keyMapper[VK_OEM_7] = Key::KEY_QUOTE;
        keyMapper[VK_OEM_COMMA] = Key::KEY_COMMA;
        keyMapper[VK_OEM_PERIOD] = Key::KEY_PERIOD;
        keyMapper[VK_OEM_2] = Key::KEY_SLASH;

        keyMapper[VK_UP] = Key::KEY_UP;
        keyMapper[VK_DOWN] = Key::KEY_DOWN;
        keyMapper[VK_LEFT] = Key::KEY_LEFT;
        keyMapper[VK_RIGHT] = Key::KEY_RIGHT;

        keyMapper[VK_LSHIFT] = Key::KEY_LEFT_SHIFT;
        keyMapper[VK_RSHIFT] = Key::KEY_RIGHT_SHIFT;
        keyMapper[VK_LCONTROL] = Key::KEY_LEFT_CONTROL;
        keyMapper[VK_RCONTROL] = Key::KEY_RIGHT_CONTROL;
        keyMapper[VK_LMENU] = Key::KEY_LEFT_ALT;
        keyMapper[VK_RMENU] = Key::KEY_RIGHT_ALT;
        keyMapper[VK_LWIN] = Key::KEY_LEFT_SUPER;
        keyMapper[VK_RWIN] = Key::KEY_RIGHT_SUPER;

        keyMapper[VK_TAB] = Key::KEY_TAB;
        keyMapper[VK_CAPITAL] = Key::KEY_CAPS_LOCK;
        keyMapper[VK_SPACE] = Key::KEY_SPACE;
        keyMapper[VK_BACK] = Key::KEY_BACKSPACE;
        keyMapper[VK_RETURN] = Key::KEY_ENTER;
        keyMapper[VK_ESCAPE] = Key::KEY_ESCAPE;
        keyMapper[VK_INSERT] = Key::KEY_INSERT;
        keyMapper[VK_DELETE] = Key::KEY_DELETE;
        keyMapper[VK_HOME] = Key::KEY_HOME;
        keyMapper[VK_END] = Key::KEY_END;
        keyMapper[VK_PRIOR] = Key::KEY_PAGE_UP;
        keyMapper[VK_NEXT] = Key::KEY_PAGE_DOWN;
        keyMapper[VK_NUMLOCK] = Key::KEY_NUM_LOCK;


        for (int i = 'A'; i <= 'Z'; ++i) {
            keyMapper[i] = static_cast<Key>(static_cast<int>(KeyCategory::ALPHABET) + i - 'A');
        }

        keyMapper[VK_PRINT] = Key::KEY_PRINT_SCREEN;
        keyMapper[VK_SCROLL] = Key::KEY_SCROLL_LOCK;
        keyMapper[VK_PAUSE] = Key::KEY_PAUSE;
    }


    HMENU Window::initMenu() {
        const HMENU hMenubar = CreateMenu();
        menubar.emplace(hMenubar);
        return hMenubar;
    }
    void Window::setClientSize(const int width, const int height) const {
        const RECT rect = {0, 0, width, height};
        RECT adjusted = rect;
        const auto &[top, left, bottom, right] = initializerSettings.paddings;
        AdjustWindowRect(&adjusted, WS_OVERLAPPEDWINDOW | WS_SYSMENU, initializerSettings.useMenubar);

        SetWindowPos(mainWindow, nullptr, 0, 0, adjusted.right - adjusted.left + left + right,
                     adjusted.bottom - adjusted.top + bottom + top, SWP_NOMOVE | SWP_NOZORDER);
        setRenderWindowSize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    }

    void Window::setRenderWindowSize(const uint32_t width, const uint32_t height) const {
        const auto &paddings = initializerSettings.paddings;
        SetWindowPos(renderWindow, nullptr, paddings.left, paddings.top, width, height, SWP_NOZORDER);
    }

    VkExtent2D Window::calculateClientSizeFromMainWindow() const {
        RECT rect;
        const auto &[top, left, bottom, right] = initializerSettings.paddings;
        GetClientRect(mainWindow, &rect);

        const uint32_t width = rect.right - rect.left - right - left;
        const uint32_t height = rect.bottom - rect.top - bottom - top;

        if (width <= 0 || height <= 0) {
            return {0, 0};
        }

        return {width, height};
    }

    void Window::resolveWindowResizeEnd() const {
        auto [width, height] = calculateClientSizeFromMainWindow();
        setRenderWindowSize(width, height);
    }

    void Window::prepareWindow() const {
        ShowWindow(mainWindow, SW_SHOW);
        UpdateWindow(mainWindow);
        SetWindowLongPtr(mainWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        SetWindowLongPtr(renderWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }
    void Window::createMainWindow(const HMENU hMenubar) {

        const auto name = s2ws(initializerSettings.name);
        const auto icon = s2ws(initializerSettings.icon);

        const WNDCLASSEX wcex = {
                .cbSize = sizeof(WNDCLASSEX),
                .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
                .lpfnWndProc = MainWindowProc,
                .hInstance = GetModuleHandle(nullptr),
                .hIcon = static_cast<HICON>(
                        LoadImage(GetModuleHandle(nullptr), icon.data(), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)),
                .hCursor = nullptr,
                .hbrBackground = nullptr,
                .lpszMenuName = nullptr,
                .lpszClassName = MAIN_CLASS_NAME,
        };

        if (!RegisterClassEx(&wcex))
            throw exception_init("Failed to register class : Main Window");

        const DWORD style = initializerSettings.resizable ? WS_OVERLAPPEDWINDOW
                                                          : WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

        mainWindow = CreateWindowEx(0, MAIN_CLASS_NAME, name.data(), style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                    CW_USEDEFAULT, nullptr, hMenubar, nullptr, nullptr);


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

        renderWindow = CreateWindowEx(0, RENDER_CLASS_NAME, RENDER_CLASS_NAME, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, mainWindow, nullptr,
                                      nullptr, nullptr);

        if (!renderWindow) {
            throw exception_init("Failed to create render window!\n");
        }


        DragAcceptFiles(renderWindow, initializerSettings.filedrop);
    }
    bool Window::canRenderable() {
        RECT rect;
        GetClientRect(renderWindow, &rect);
        return !isResizing && IsWindow(renderWindow) && IsWindowVisible(renderWindow) && !IsIconic(renderWindow) &&
               rect.bottom - rect.top > 0 && rect.right - rect.left > 0;
    }

    VkSurfaceKHR Window::createSurface(VkInstance instance) {
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        const VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                                                               .pNext = nullptr,
                                                               .flags = 0,
                                                               .hinstance = GetModuleHandle(nullptr),
                                                               .hwnd = renderWindow};
        if (allocator::invoke(vkCreateWin32SurfaceKHR, instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
            throw exception_init("failed to create window surface!");
        }
        return surface;
    }
    void Window::init() {
        SetProcessDPIAware();
        initWindow();
    }

    void Window::start() {
        PlatformWindowBase::start();
        MSG message;
        using namespace std::chrono;
        auto started = high_resolution_clock::now();

        while (true) {
            if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessage(&message);

                if (message.message == WM_QUIT) {
                    break;
                }
            }
            auto now = high_resolution_clock::now();
            const auto elapsed = duration_cast<milliseconds>(now - started);

            if (canRenderable() && static_cast<float>(elapsed.count()) > 1000 / initializerSettings.framerate) {
                started = now;
                eventSystem.applicationLifecycle.onUpdate.invoke();
            }
        }
    }


    void Window::cleanup() {}


} // namespace merutilm::mvk
#endif
