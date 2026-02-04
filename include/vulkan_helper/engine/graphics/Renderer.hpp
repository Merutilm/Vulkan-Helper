//
// Created by Merutilm on 2025-09-08.
//

#pragma once
#include <vulkan_helper/engine/configurator/PipelineConfigurator.hpp>
#include <vulkan_helper/engine/executor/ScopedCommandBufferExecutor.hpp>

namespace merutilm::vkh {
    struct Renderer : Handler {
        Engine & engine;
        WindowContext & wc;

        uint32_t frameIndex = 0;
        bool pipelineInitialized = false;
        std::vector<PipelineConfigurator> configurators = {};

        explicit Renderer(Engine & engine, uint32_t windowContextIndex);

        ~Renderer() override;

        Renderer(const Renderer &windowContext) = delete;

        Renderer(Renderer &&) noexcept = default;

        Renderer &operator=(Renderer &&) = delete;

        Renderer &operator=(const Renderer &) = delete;


        [[nodiscard]] uint32_t getFrameIndex() const;

        void finishPipelineInitialization();

        void execute();

    private:
        virtual void beforeCmdRender() = 0;

        virtual void cmdRender(uint32_t swapchainImageIndex) = 0;
    };


}
