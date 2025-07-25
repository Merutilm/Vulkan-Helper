//
// Created by Merutilm on 2025-07-07.
//

#include "Application.hpp"

#include <iostream>
#include <glm/glm.hpp>

#include "../configurator/BasicRenderPassConfigurator.hpp"
#include "../configurator/GeneralPipelineConfigurator.hpp"
#include "../configurator/TemplatePipelineConfigurator.hpp"
#include "../executor/RenderPassExecutor.hpp"
#include "../repo/PipelineLayoutRepo.hpp"

namespace merutilm::vkh {
    Application::Application() {
        start();
    }

    Application::~Application() {
        quit();
    }

    void Application::start() {
        const WindowCreateInfo windowCreateInfo = {
            .windowAttachmentName = MAIN_WINDOW_ATTACHMENT_NAME,
            .windowTitleName = L"Test",
            .width = 1280,
            .height = 720,
            .framerate = 120.0f
        };
        auto core = Core::createCore();
        core->createWindow(windowCreateInfo);
        engine = std::make_unique<Engine>(std::move(core));
        engine->attachRenderContext(std::make_unique<RenderContext>(engine->getCore(), MAIN_WINDOW_ATTACHMENT_NAME, engine->getCommandPool(), BasicRenderPassConfigurator{}));

        shaderProgram = GeneralPipelineConfigurator::createShaderProgram<TemplatePipelineConfigurator>(*engine, BasicRenderPassConfigurator::SUBPASS_PRIMARY, "../shaders/template.vert", "../shaders/template.frag");

        const HCURSOR hCursor = LoadCursor(nullptr, IDC_ARROW);

        Window &window = *engine->getCore().getWindowContext(windowCreateInfo.windowAttachmentName).window;

        window.setListener(WM_MOUSEMOVE, [hCursor](const Window &) {
            SetCursor(hCursor);
            return static_cast<LRESULT>(true);
        });
        window.setListener(WM_SIZE, [this](const Window &) {
            resolveWindowResize();
            return static_cast<LRESULT>(0);
        });

        window.appendRenderer([this] { drawFrame(); });
        window.start();
    }



    void Application::drawFrame() {
        Core &core = engine->getCore();
        const RenderContext &renderContext = engine->getRenderContext();
        if (core.getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).window->isUnrenderable()) {
            return;
        }
        changeFrameIndex();
        const Swapchain &swapchain = *core.getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).swapchain;
        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        const VkFence currentFence = renderContext.syncObject->getFence(currentFrame);
        const VkSemaphore imageAvailableSemaphore = renderContext.syncObject->getImageAvailableSemaphore(currentFrame);
        const VkSwapchainKHR swapchainHandle = swapchain.getSwapchainHandle();


        vkWaitForFences(device, 1, &currentFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &currentFence);

        uint32_t imageIndex = 0;
        vkAcquireNextImageKHR(device, swapchainHandle, UINT64_MAX, imageAvailableSemaphore,
                              nullptr, &imageIndex);


        //COMMAND SCOPE START
        {
            const auto renderPassExecutor = RenderPassExecutor(core, renderContext,
                                                               currentFrame, imageIndex);
            const VkCommandBuffer cbh = renderContext.commandBuffer->getCommandBufferHandle(currentFrame);
            const auto sets = shaderProgram->getPipeline().getPipelineManager().getDescriptorSets(
                currentFrame);
            vkCmdBindPipeline(cbh, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderProgram->getPipeline().getPipelineHandle());
            vkCmdBindDescriptorSets(cbh, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    shaderProgram->getPipeline().getPipelineManager().getLayout().getLayoutHandle(), 0, static_cast<uint32_t>(sets.size()), sets.data(), 0,
                                    nullptr);

            auto [width, height] = swapchain.populateSwapchainExtent();
            swapchain.matchViewportAndScissor(cbh);

            shaderProgram->getPipeline().getPipelineManager().getLayout().getPipelineLayoutManager().getPCM("push").set("resolution", glm::vec2{static_cast<float>(width), static_cast<float>(height)});
            shaderProgram->getPipeline().getPipelineManager().getLayout().push(cbh);
            shaderProgram->update(currentFrame);
            shaderProgram->draw(cbh, currentFrame);

        } // COMMAND SCOPE END
    }

    void Application::changeFrameIndex() {
        ++currentFrame %= engine->getCore().getPhysicalDevice().getMaxFramesInFlight();
    }

    void Application::resolveWindowResize() {
        Core &core = engine->getCore();
        const RenderContext &renderContext = engine->getRenderContext();
        if (core.getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).window->isUnrenderable()) {
            return;
        }
        vkDeviceWaitIdle(core.getLogicalDevice().getLogicalDeviceHandle());
        renderContext.framebuffer->detach();
        renderContext.renderPass->detach();
        core.getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).swapchain->recreate();
        renderContext.renderPass->attach();
        renderContext.framebuffer->attach();
        currentFrame = 0;
        drawFrame();
    }

    void Application::quit() {
        vkDeviceWaitIdle(engine->getCore().getLogicalDevice().getLogicalDeviceHandle());
        shaderProgram = nullptr;
        engine = nullptr;
    }
}
