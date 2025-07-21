//
// Created by Merutilm on 2025-07-19.
//

#pragma once
#include <memory>

#include "Core.hpp"
#include "../context/RenderContext.hpp"
#include "../obj/CommandPool.hpp"
#include "../repo/Repositories.hpp"

namespace merutilm::vkh {
    class Engine final : public Handler {
        std::unique_ptr<Core> core = nullptr;
        std::unique_ptr<Repositories> repositories = nullptr;
        std::unique_ptr<CommandPool> commandPool = nullptr;
        std::unique_ptr<RenderContext> renderContext = nullptr;

    public:
        explicit Engine(std::unique_ptr<Core> &&core);

        ~Engine() override;

        Engine(const Engine &) = delete;

        Engine &operator=(const Engine &) = delete;

        Engine(Engine &&) = delete;

        Engine &operator=(Engine &&) = delete;

        void attachRenderContext(std::unique_ptr<RenderContext> &&renderContext);

        Core &getCore() const { return *core; }

        Repositories &getRepositories() const { return *repositories; }

        CommandPool &getCommandPool() const { return *commandPool; }

        RenderContext &getRenderContext() const { return *renderContext; }

    private:
        void attach() override;

        std::unique_ptr<RenderContext> detachRenderContext();

        void detach() override;
    };
}
