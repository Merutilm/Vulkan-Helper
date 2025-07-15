//
// Created by Merutilm on 2025-07-07.
//

#pragma once
#include <cstdint>
#include <functional>
#include <windows.h>
#include "../handle/Handler.hpp"

namespace merutilm::vkh {
    class Window final : public Handler {
        static constexpr auto CLASS_NAME = L"Vulkan";
        const wchar_t *windowName = nullptr;
        const float framerate;
        const uint32_t width;
        const uint32_t height;
        HWND window = nullptr;
        std::unordered_map<UINT, std::vector<std::function<void(const Window &)>>> listeners = {};
        std::vector<std::function<void()>> renderers = {};


    public:
        explicit Window(const wchar_t *windowName, float framerate, uint32_t width, uint32_t height);

        ~Window() override;

        Window(Window &) = delete;

        Window(Window &&) = delete;

        Window &operator=(const Window &) = delete;

        Window &operator=(Window &&) = delete;

        HWND getWindowHandle() const {return window;}

        template <typename F> requires std::is_invocable_r_v<void, F, const Window &>
        void appendListener(UINT message, F&& func);

        template <typename F> requires std::is_invocable_r_v<void, F>
        void appendRenderer(F&& func);

        void start() const;

    private:
        void init() override;

        static void registerClass();

        void createWindow();

        static LRESULT CALLBACK winProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

        void destroy() override;

        void clear() { listeners.clear(); renderers.clear(); }

        void runListeners(UINT message);


    };

    template<typename F> requires std::is_invocable_r_v<void, F, const Window &>
    void Window::appendListener(const UINT message, F&& func) {
        listeners[message].emplace_back(std::forward<F>(func));
    }

    template<typename F> requires std::is_invocable_r_v<void, F>
    void Window::appendRenderer(F &&func) {
        renderers.emplace_back(std::forward<F>(func));
    }

}
