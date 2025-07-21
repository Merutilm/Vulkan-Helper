//
// Created by Merutilm on 2025-07-07.
//

#include "Application.hpp"

#include <iostream>
#include <glm/glm.hpp>

#include "UniformBuffer.hpp"
#include "../base/BasicShaderProgram.hpp"
#include "../base/TemplateShaderProgram.hpp"
#include "../executor/RenderPassExecutor.hpp"
#include "../repo/DescriptorSetLayoutRepo.hpp"
#include "../repo/PipelineLayoutRepo.hpp"
#include "../struct/Vertex.hpp"
#include "../repo/ShaderModuleRepo.hpp"

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
        engine->attachRenderContext(createRenderContext());

        shaderProgram = BasicShaderProgram::createProgram<TemplateShaderProgram>(*engine, "Sample", "../shaders/template.vert", "../shaders/template.frag");


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

    std::unique_ptr<RenderContext> Application::createRenderContext() const {
        auto renderPassManager = std::make_unique<RenderPassManager>(engine->getCore());
        renderPassManager->appendSubpass("Sample");
        renderPassManager->appendAttachment("col", RenderPassAttachmentType::COLOR, {
                                                .flags = 0,
                                                .format = VK_FORMAT_R8G8B8A8_SRGB,
                                                .samples = VK_SAMPLE_COUNT_1_BIT,
                                                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                                                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                                            });
        renderPassManager->appendAttachment("depth_stencil", RenderPassAttachmentType::DEPTH_STENCIL, {
                                                .flags = 0,
                                                .format = VK_FORMAT_D32_SFLOAT,
                                                .samples = VK_SAMPLE_COUNT_1_BIT,
                                                .loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                                            }, ImageInitInfo{
                                                .imageType = VK_IMAGE_TYPE_2D,
                                                .imageViewType = VK_IMAGE_VIEW_TYPE_2D,
                                                .imageFormat = VK_FORMAT_D32_SFLOAT,
                                                .extent = {},
                                                .mipLevels = 1,
                                                .arrayLayers = 1,
                                                .samples = VK_SAMPLE_COUNT_1_BIT,
                                                .imageTiling = VK_IMAGE_TILING_OPTIMAL,
                                                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                                                .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                            });
        renderPassManager->appendDependency({
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = renderPassManager->getSubpassIndex("Sample"),
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dependencyFlags = 0
        });
        renderPassManager->appendDependency({
            .srcSubpass = renderPassManager->getSubpassIndex("Sample"),
            .dstSubpass = VK_SUBPASS_EXTERNAL,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dstAccessMask = 0,
            .dependencyFlags = 0
        });

        return std::make_unique<RenderContext>(engine->getCore(), MAIN_WINDOW_ATTACHMENT_NAME, engine->getCommandPool(),
                                               std::move(renderPassManager));
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
            const VkDescriptorSet sets = shaderProgram->getPipeline().getPipelineManager().getDescriptor(0).getDescriptorSetHandle(
                currentFrame);
            vkCmdBindPipeline(cbh, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderProgram->getPipeline().getPipelineHandle());
            vkCmdBindDescriptorSets(cbh, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    shaderProgram->getPipeline().getPipelineManager().getLayout().getLayoutHandle(), 0, 1, &sets, 0,
                                    nullptr);
            const auto [width, height] = swapchain.populateSwapchainExtent();
            const VkViewport viewport = {
                .x = 0,
                .y = 0,
                .width = static_cast<float>(width),
                .height = static_cast<float>(height),
                .minDepth = 0,
                .maxDepth = 1
            };
            const VkRect2D scissor = {
                .offset = {0, 0},
                .extent = {width, height}
            };


            vkCmdSetViewport(cbh, 0, 1, &viewport);
            vkCmdSetScissor(cbh, 0, 1, &scissor);

            shaderProgram->getPipeline().getPipelineManager().getLayout().getPipelineLayoutManager().getPCM("push").set(
                "resolution", glm::vec2{static_cast<float>(width / 2), static_cast<float>(height / 2)});

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
