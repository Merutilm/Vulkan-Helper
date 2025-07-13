//
// Created by Merutilm on 2025-07-07.
//

#pragma once
#include <memory>

#include "../objhandler/Handler.hpp"

#include "Engine.hpp"
#include "CommandPool.hpp"
#include "CommandBuffer.hpp"
#include "RenderPass.hpp"


namespace merutilm::vkh {
    class Application final : public Handler{

        std::unique_ptr<Engine> engine = nullptr;
        std::unique_ptr<CommandPool> commandPool = nullptr;
        std::unique_ptr<CommandBuffer> commandBuffer = nullptr;
        std::unique_ptr<RenderPass> renderPass = nullptr;

    public:
        explicit Application();

        ~Application() override;

        Application(const Application &) = delete;

        Application &operator=(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(Application &&) = delete;

        void start() const;

    private:
        void init() override;

        void destroy() override;
    };
}
