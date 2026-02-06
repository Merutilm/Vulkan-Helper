//
// Created by Merutilm on 2026-02-06.
//

#pragma once
#include "handle/EngineHandler.hpp"
#include "window/WindowInitializerSettings.hpp"
namespace merutilm::vkh {


    struct Application : Handler {

        Engine engine{};

        explicit Application(WindowInitializerSettings &&settings);

        ~Application() override;

        Application(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(const Application &) = delete;

        Application &operator=(Application &&) = delete;

    private:
        void init() override;

        void cleanup() override;
    };


} // namespace merutilm::vkh
