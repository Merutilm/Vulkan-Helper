//
// Created by Merutilm on 2025-09-08.
//

#pragma once
#include <vulkan_helper/configurator/PipelineConfigurator.hpp>
#include <vulkan_helper/base/vkh.hpp>
#include <vulkan_helper/executor/ScopedCommandBufferExecutor.hpp>
#include <vulkan_helper/util/SwapchainUtils.hpp>

namespace merutilm::vkh {
    struct RendererAbstract : public Handler {
        EngineRef engine;
        WindowContextRef wc;

        uint32_t frameIndex = 0;
        bool pipelineInitialized = false;
        std::vector<PipelineConfigurator> configurators = {};

        explicit RendererAbstract(EngineRef engine, const uint32_t windowContextIndex);

        ~RendererAbstract() override;

        RendererAbstract(const RendererAbstract &windowContext) = delete;

        RendererAbstract(RendererAbstract &&) = delete;

        RendererAbstract &operator=(RendererAbstract &&) = delete;

        RendererAbstract &operator=(const RendererAbstract &) = delete;


        [[nodiscard]] uint32_t getFrameIndex() const;

        void finishPipelineInitialization() const;;

        void execute();

    private:
        virtual void beforeCmdRender() = 0;

        virtual void cmdRender(uint32_t swapchainImageIndex) = 0;
    };

    using Renderer = std::unique_ptr<RendererAbstract>;
    using RendererPtr = RendererAbstract *;
    using RendererRef = RendererAbstract &;
}
