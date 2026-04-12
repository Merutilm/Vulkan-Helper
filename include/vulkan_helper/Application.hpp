//
// Created by Merutilm on 2026-02-06.
//

#pragma once
#include "engine/configurator/PipelineConfigurator.hpp"
#include "handle/EngineHandler.hpp"
namespace merutilm::mvk {


    struct Application : Handler {

        Engine engine{};

        explicit Application();

        ~Application() override;

        Application(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(const Application &) = delete;

        Application &operator=(Application &&) = delete;

    private:
        void init() override;

        void cleanup() override;
    };


} // namespace merutilm::mvk
