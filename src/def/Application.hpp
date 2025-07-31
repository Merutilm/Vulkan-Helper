//
// Created by Merutilm on 2025-07-07.
//

#pragma once
#include <memory>

#include "Core.hpp"
#include "../configurator/GeneralPipelineConfigurator.hpp"
#include "Engine.hpp"


namespace merutilm::vkh {
    class Application final{

        uint32_t currentFrame = 0;
        inline static const std::string MAIN_WINDOW_ATTACHMENT_NAME = "Main";

        std::unique_ptr<Engine> engine = nullptr;
        std::vector<std::unique_ptr<GeneralPipelineConfigurator>> shaderPrograms = {};


    public:
        explicit Application();

        ~Application();

        Application(const Application &) = delete;

        Application &operator=(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(Application &&) = delete;

    private:
        void start();

        void drawFrame();

        void changeFrameIndex();

        void resolveWindowResize();

        void quit();
    };
}
