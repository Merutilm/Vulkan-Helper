//
// Created by Merutilm on 2025-07-07.
//

#pragma once
#include <vulkan_helper/base/config.hpp>
#include <vulkan_helper/base/pch.hpp>

namespace merutilm::vkh {

    class GraphicsContextWindow;



    class GraphicsContextWindow final {
        using Listeners = std::unordered_map<UINT, std::function<LRESULT(GraphicsContextWindow &, HWND, WPARAM, LPARAM)> > ;
        HWND window = nullptr;
        float framerate = config::INITIAL_FPS;
        Listeners listeners = {};
        std::vector<std::function<void()> > renderers = {};

    public:
        explicit GraphicsContextWindow(HWND window);


        ~GraphicsContextWindow() = default;

        GraphicsContextWindow(GraphicsContextWindow &) = delete;

        GraphicsContextWindow(GraphicsContextWindow &&) noexcept = default;

        GraphicsContextWindow &operator=(const GraphicsContextWindow &) = delete;

        GraphicsContextWindow &operator=(GraphicsContextWindow &&) = delete;

        HWND getWindowHandle() const { return window; }

        void setFramerate(const float framerate) { this->framerate = framerate; }

        void renderOnce() const;

        [[nodiscard]] bool isUnrenderable() const {
            RECT rect;
            GetClientRect(window, &rect);
            return !IsWindow(window) || !IsWindowVisible(window) || IsIconic(window) || rect.bottom - rect.top <= 0 ||
                   rect.right - rect.left <= 0;
        }

        template<typename F> requires std::is_invocable_r_v<LRESULT, F, GraphicsContextWindow &,  HWND, WPARAM,LPARAM>
        void setListener(UINT message, F &&func);

        template<typename F> requires std::is_invocable_r_v<void, F>
        void appendRenderer(F &&func);

        void start() const;

        const Listeners &getListeners() const {return listeners;};

    private:
        void clear() {
            listeners.clear();
            renderers.clear();
        }

    };

    template<typename F> requires std::is_invocable_r_v<LRESULT, F, GraphicsContextWindow &, HWND, WPARAM, LPARAM>
    void GraphicsContextWindow::setListener(const UINT message, F &&func) {
        listeners[message] = std::forward<F>(func);
    }

    template<typename F> requires std::is_invocable_r_v<void, F>
    void GraphicsContextWindow::appendRenderer(F &&func) {
        renderers.emplace_back(std::forward<F>(func));
    }


}
