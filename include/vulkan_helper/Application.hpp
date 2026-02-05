//
// Created by Merutilm on 2026-02-06.
//

#pragma once
#include "engine/Engine.hpp"
namespace merutilm::vkh {

    struct Application : Handler{
        Engine engine;

        Application() = default;

        ~Application() override = default;

        Application(const Application &) = delete;

        Application &operator=(const Application &) = delete;

        Application(Application &&) noexcept = default;

        Application &operator=(Application &&) noexcept = delete;

        template<typename T> requires std::is_base_of_v<Application, T>
        static T start();
    };

    template<typename T> requires std::is_base_of_v<Application, T>
    T Application::start() {
        T app;
        //TODO some logics
        return app;
    }

} // namespace merutilm::vkh
