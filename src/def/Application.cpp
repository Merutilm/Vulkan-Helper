//
// Created by Merutilm on 2025-07-07.
//

#include "Application.hpp"

#include <iostream>
#include <glm/glm.hpp>

#include "../configurator/BasicRenderContextConfigurator.hpp"
#include "../configurator/GeneralPipelineConfigurator.hpp"
#include "../configurator/Template1PipelineConfigurator.hpp"
#include "../configurator/Template2PipelineConfigurator.hpp"
#include "../executor/RenderPassExecutor.hpp"
#include "../util/Presenter.hpp"

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
        const auto &swapchain = *engine->getCore().getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).swapchain;
        auto configurator = std::make_unique<BasicRenderContextConfigurator>(engine->getCore(), swapchain);

        engine->attachRenderContext(std::make_unique<RenderContext>(engine->getCore(),
                                                                    swapchain.populateSwapchainExtent(),
                                                                    std::move(configurator)));

        shaderPrograms.emplace_back(GeneralPipelineConfigurator::createShaderProgram<Template1PipelineConfigurator>(
            *engine, BasicRenderContextConfigurator::SUBPASS_PRIMARY, "../shaders/template.vert",
            "../shaders/template-1.frag"));
        shaderPrograms.emplace_back(GeneralPipelineConfigurator::createShaderProgram<Template2PipelineConfigurator>(
            *engine, BasicRenderContextConfigurator::SUBPASS_SECONDARY, "../shaders/template.vert",
            "../shaders/template-2.frag"
        ));

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
        const VkFence currentFence = engine->getSyncObject().getFence(currentFrame);
        const VkSemaphore imageAvailableSemaphore = engine->getSyncObject().getImageAvailableSemaphore(currentFrame);
        const VkSwapchainKHR swapchainHandle = swapchain.getSwapchainHandle();


        vkWaitForFences(device, 1, &currentFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &currentFence);

        uint32_t imageIndex = 0;
        vkAcquireNextImageKHR(device, swapchainHandle, UINT64_MAX, imageAvailableSemaphore,
                              nullptr, &imageIndex);

        const auto extent = swapchain.populateSwapchainExtent();

        DescriptorUpdateQueue queue = {};

        for (const auto &shaderProgram: shaderPrograms) {
            shaderProgram->updateQueue(queue, currentFrame, imageIndex, extent.width, extent.height);
        }

        std::vector<VkWriteDescriptorSet> writeDescriptorSets(queue.size());
        std::ranges::transform(queue, writeDescriptorSets.begin(), [](const DescriptorUpdateContext &context) {
            return context.writeSet;
        });

        vkUpdateDescriptorSets(core.getLogicalDevice().getLogicalDeviceHandle(),
                               static_cast<uint32_t>(writeDescriptorSets.size()),
                               writeDescriptorSets.data(), 0, nullptr);

        //COMMAND SCOPE START
        {
            const auto renderPassExecutor = RenderPassExecutor(*engine, renderContext, extent,
                                                               currentFrame, imageIndex);

            const VkCommandBuffer cbh = engine->getCommandBuffer().getCommandBufferHandle(currentFrame);
            swapchain.matchViewportAndScissor(cbh);

            for (int i = 0; i < shaderPrograms.size(); ++i) {
                shaderPrograms[i]->getPipeline().bind(cbh, currentFrame);
                shaderPrograms[i]->render(cbh, currentFrame, extent.width, extent.height);
                if (i < shaderPrograms.size() - 1) {
                    vkCmdNextSubpass(cbh, VK_SUBPASS_CONTENTS_INLINE);
                }
            }
        } // COMMAND SCOPE END

        Presenter::present(*engine, swapchainHandle, currentFrame, imageIndex);
    }

    void Application::changeFrameIndex() {
        ++currentFrame %= engine->getCore().getPhysicalDevice().getMaxFramesInFlight();
    }

    void Application::resolveWindowResize() {
        Core &core = engine->getCore();
        RenderContext &renderContext = engine->getRenderContext();
        if (core.getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).window->isUnrenderable()) {
            return;
        }
        vkDeviceWaitIdle(core.getLogicalDevice().getLogicalDeviceHandle());

        Swapchain &swapchain = *core.getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).swapchain;
        swapchain.recreate();
        renderContext.recreate(swapchain.populateSwapchainExtent());
        drawFrame();
    }

    void Application::quit() {
        vkDeviceWaitIdle(engine->getCore().getLogicalDevice().getLogicalDeviceHandle());
        shaderPrograms.clear();
        engine = nullptr;
    }
}
