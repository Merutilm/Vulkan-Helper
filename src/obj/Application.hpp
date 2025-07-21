//
// Created by Merutilm on 2025-07-07.
//

#pragma once
#include <memory>

#include "../base/Core.hpp"
#include "../base/BasicShaderProgram.hpp"
#include "../base/Engine.hpp"


namespace merutilm::vkh {
    class Application final{

        uint32_t currentFrame = 0;
        inline static const std::string MAIN_WINDOW_ATTACHMENT_NAME = "Main";

        std::unique_ptr<Engine> engine = nullptr;
        std::unique_ptr<BasicShaderProgram> shaderProgram = nullptr;


    public:
        explicit Application();

        ~Application();

        Application(const Application &) = delete;

        Application &operator=(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(Application &&) = delete;

    private:
        void start();

        std::unique_ptr<RenderContext> createRenderContext() const;

        void drawFrame();

        void changeFrameIndex();

        void resolveWindowResize();

        void quit();
    };
}
