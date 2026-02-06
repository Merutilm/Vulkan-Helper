//
// Created by Merutilm on 2026-02-06.
//

#pragma once
#ifdef WIN32
#include <Windows.h>

#include <vulkan_helper/window/PlatformWindowBase.hpp>
#include "Menubar.hpp"

namespace merutilm::vkh {

    class Window final : public PlatformWindowBase {
        HWND mainWindow = nullptr;
        HWND renderWindow = nullptr;
        std::optional<Menubar> menubar;
        static constexpr auto RENDER_CLASS_NAME = "RenderWindow";
        static constexpr auto MAIN_CLASS_NAME = "MainWindow";

    public:
        explicit Window(WindowInitializerSettings &&settings);

        ~Window() override;

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        Window(Window &&) noexcept = default;
        Window &operator=(Window &&) noexcept = delete;

    private:

        static LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK RenderWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        void initWindow();

        HMENU initMenu();

        void setClientSize(int width, int height) const;

        void adjustClient(const RECT &rect) const;

        void resolveWindowResizeEnd() const;

        void prepareWindow() const;

        void createMainWindow(HMENU hMenubar);

        void createRenderWindow();

        void init() override;
        void cleanup() override;
    };
} // namespace merutilm::vkh
#endif
