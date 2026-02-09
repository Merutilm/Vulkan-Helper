//
// Created by Merutilm on 2026-02-06.
//

#pragma once
#ifdef WIN32
#include <Windows.h>
#include <vulkan_helper/engine/window/PlatformWindowBase.hpp>
#include "Menubar.hpp"

namespace merutilm::vkh {

    class Window final : public PlatformWindowBase {
        HWND mainWindow = nullptr;
        HWND renderWindow = nullptr;
        std::optional<Menubar> menubar;
        static constexpr auto RENDER_CLASS_NAME = L"RenderWindow";
        static constexpr auto MAIN_CLASS_NAME = L"MainWindow";

    public:
        explicit Window(WindowInitializerSettings &&settings);

        ~Window() override;

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        Window(Window &&) = delete;
        Window &operator=(Window &&) noexcept = delete;

        HWND getMainWindow() const { return mainWindow;}
        HWND getRenderWindow() const { return renderWindow;}

    private:

        static LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK RenderWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        void initWindow();

        HMENU initMenu();

        void setClientSize(int width, int height) const;

        void setRenderWindowSize(uint32_t width, uint32_t height) const;

        void resolveWindowResizeEnd() const;

        void prepareWindow() const;

        void createMainWindow(HMENU hMenubar);

        void createRenderWindow();

        static std::string ws2s(const std::wstring &str);

        static std::wstring s2ws(const std::string &str);

        VkExtent2D calculateClientSizeFromMainWindow() const override;

        bool canRenderable() override;

        void start() override;

        void init() override;

        void cleanup() override;

        void generateKeyMapper() override;

    public:
        VkSurfaceKHR createSurface(VkInstance instance) override;

    };
} // namespace merutilm::vkh
#endif
