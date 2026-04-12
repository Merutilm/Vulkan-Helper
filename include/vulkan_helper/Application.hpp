//
// Created by Merutilm on 2026-02-06.
//

#pragma once
#include "engine/buffer/IndexBuffer.hpp"
#include "engine/buffer/VertexBuffer.hpp"
#include "handle/EngineHandler.hpp"
namespace merutilm::mvk {


    struct Application : Handler {

        Engine engine{};
        std::unique_ptr<VertexBuffer> vertexBufferPP;
        std::unique_ptr<IndexBuffer> indexBufferPP;

        explicit Application();

        ~Application() override;

        Application(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(const Application &) = delete;

        Application &operator=(Application &&) = delete;

    protected:
        void init() override;

        void cleanup() override;
    };


} // namespace merutilm::mvk
