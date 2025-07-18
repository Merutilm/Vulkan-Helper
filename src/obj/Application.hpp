//
// Created by Merutilm on 2025-07-07.
//

#pragma once
#include <memory>

#include "../context/Engine.hpp"
#include "CommandPool.hpp"
#include "Pipeline.hpp"
#include "VertexBuffer.hpp"
#include "../context/RenderContext.hpp"
#include "../repo/Repositories.hpp"


namespace merutilm::vkh {
    class Application final{

        uint32_t currentFrame = 0;
        inline static const std::string MAIN_WINDOW_ATTACHMENT_NAME = "Main";

        std::unique_ptr<Engine> engine = nullptr;
        std::unique_ptr<CommandPool> commandPool = nullptr;
        std::unique_ptr<Repositories> repositories = nullptr;
        std::unique_ptr<RenderContext> renderContext = nullptr;
        std::unique_ptr<VertexBuffer> vertexBuffer = nullptr;
        std::unique_ptr<Pipeline> pipeline = nullptr;

    public:
        explicit Application();

        ~Application();

        Application(const Application &) = delete;

        Application &operator=(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(Application &&) = delete;

    private:
        void start();

        void createRenderContext();

        void drawFrame();

        void changeFrameIndex();

        void resolveResize();

        void quit();
    };
}
