//
// Created by Merutilm on 2025-07-07.
//

#include "Application.hpp"

#include <iostream>
#include <glm/glm.hpp>

#include "UniformBuffer.hpp"
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
        engine = Engine::createEngine();
        engine->attachWindow(windowCreateInfo);
        commandPool = std::make_unique<CommandPool>(*engine);
        createRenderContext();


        auto vertexBufferManager = std::make_unique<BufferObjectManager>();
        const std::vector<Vertex> vertices{
            {{0.0f, -0.5f, 0.0f}, {1, 0, 0}},
            {{0.5f, 0.5f, 0.0f}, {0, 1, 0}},
            {{-0.5f, 0.5f, 0.0f}, {0, 0, 1}}
        };

        vertexBufferManager->add("vertices", vertices);
        vertexBuffer = std::make_unique<VertexBuffer>(*engine, std::move(vertexBufferManager));


        repositories = std::make_unique<Repositories>();
        repositories->addRepository<DescriptorSetLayoutRepo>(*engine);
        repositories->addRepository<PipelineLayoutRepo>(*engine);
        repositories->addRepository<ShaderModuleRepo>(*engine);


        auto descriptorLayoutManager = std::make_unique<DescriptorSetLayoutManager>();
        descriptorLayoutManager->append({
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .stage = VK_SHADER_STAGE_FRAGMENT_BIT
        });
        auto &descriptorLayout = repositories->getRepository<DescriptorSetLayoutRepo>()->pick(std::move(descriptorLayoutManager));

        auto pipelineLayoutManager = std::make_unique<PipelineLayoutManager>();
        pipelineLayoutManager->appendDescriptorSetLayout(descriptorLayout.getLayoutHandle());

        auto &pipelineLayout = repositories->getRepository<PipelineLayoutRepo>()->pick(std::move(pipelineLayoutManager));
        auto descManager = std::make_unique<DescriptorManager>(descriptorLayout);

        auto uploader = std::make_unique<BufferObjectManager>();
        uploader->reserve<uint32_t>("time");

        auto ubo = std::make_unique<UniformBuffer>(*engine, std::move(uploader));
        descManager->appendUBO("SS", std::move(ubo));

        auto descriptor = std::make_unique<Descriptor>(*engine, std::move(descManager));
        auto pipelineManager = std::make_unique<PipelineManager>(pipelineLayout, std::move(descriptor));
        auto &vert = repositories->getRepository<ShaderModuleRepo>()->pick("../shaders/template.vert");
        auto &frag = repositories->getRepository<ShaderModuleRepo>()->pick("../shaders/template.frag");
        pipelineManager->attachShader(&vert);
        pipelineManager->attachShader(&frag);
        pipeline = std::make_unique<Pipeline>(*engine, pipelineLayout, *renderContext->renderPass, *vertexBuffer,
                                              "Sample",
                                              std::move(pipelineManager));

        const HCURSOR hCursor = LoadCursor(nullptr, IDC_ARROW);

        Window &window = *engine->getWindowContext(windowCreateInfo.windowAttachmentName).window;

        window.setListener(WM_MOUSEMOVE, [hCursor](const Window &) {
            SetCursor(hCursor);
            return static_cast<LRESULT>(true);
        });
        window.setListener(WM_SIZE, [this](const Window &) {
            resolveResize();
            return static_cast<LRESULT>(0);
        });
        window.appendRenderer([this] { drawFrame(); });
        window.start();
    }

    void Application::createRenderContext() {
        auto renderPassManager = std::make_unique<RenderPassManager>(*engine);
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

        renderContext = std::make_unique<RenderContext>(*engine, MAIN_WINDOW_ATTACHMENT_NAME, *commandPool,
                                                        std::move(renderPassManager));
    }

    void Application::drawFrame() {
        if (engine->getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).window->isUnrenderable()) {
            return;
        }
        changeFrameIndex();
        const Swapchain &swapchain = *engine->getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).swapchain;
        const VkDevice device = engine->getLogicalDevice().getLogicalDeviceHandle();
        const VkFence currentFence = renderContext->syncObject->getFence(currentFrame);
        const VkSemaphore imageAvailableSemaphore = renderContext->syncObject->getImageAvailableSemaphore(currentFrame);
        const VkSwapchainKHR swapchainHandle = swapchain.getSwapchainHandle();


        vkWaitForFences(device, 1, &currentFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &currentFence);

        uint32_t imageIndex = 0;
        vkAcquireNextImageKHR(device, swapchainHandle, UINT64_MAX, imageAvailableSemaphore,
                              nullptr, &imageIndex);


        //COMMAND SCOPE START
        {
            const auto renderPassExecutor = RenderPassExecutor(*engine, *renderContext,
                                                               currentFrame, imageIndex);
            const VkCommandBuffer cbh = renderContext->commandBuffer->getCommandBufferHandle(currentFrame);
            const VkDescriptorSet sets = pipeline->getPipelineManager().getDescriptor().getDescriptorSetHandle(currentFrame);
            vkCmdBindPipeline(cbh, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineHandle());
            vkCmdBindDescriptorSets(cbh, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pipeline->getPipelineManager().getLayout().getLayoutHandle(), 0, 1, &sets, 0,
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
            const VkBuffer vertexBufferHandle = vertexBuffer->getBuffer(currentFrame);
            const float time = engine->getTime();
            const float sin = std::sin(time) / 2 + 0.5f;
            const float cos = std::cos(time) / 2 + 0.5f;

            vertexBuffer->set("vertices", std::vector{
                                  Vertex{{0.0f, -0.5f, 0.0f}, {cos, cos, sin}},
                                  Vertex{{0.5f, 0.5f, 0.0f}, {sin, cos, sin}},
                                  Vertex{{-0.5f, 0.5f, 0.0f}, {sin, cos, cos}}
                              });

            pipeline->getPipelineManager().getLayout().push(cbh);
            pipeline->getPipelineManager().getDescriptor().getDescriptorManager().getUBO("SS").set("time", time);
            pipeline->getPipelineManager().getDescriptor().update(currentFrame);
            vertexBuffer->update(currentFrame);

            constexpr VkDeviceSize vertexBufferOffset = 0;
            vkCmdSetViewport(cbh, 0, 1, &viewport);
            vkCmdSetScissor(cbh, 0, 1, &scissor);
            vkCmdBindVertexBuffers(cbh, 0, 1, &vertexBufferHandle, &vertexBufferOffset);
            vkCmdDraw(cbh, 3, 1, 0, 0);
        } // COMMAND SCOPE END
    }

    void Application::changeFrameIndex() {
        ++currentFrame %= engine->getPhysicalDevice().getMaxFramesInFlight();
    }

    void Application::resolveResize() {
        if (engine->getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).window->isUnrenderable()) {
            return;
        }
        vkDeviceWaitIdle(engine->getLogicalDevice().getLogicalDeviceHandle());
        renderContext->framebuffer->detach();
        renderContext->renderPass->detach();
        engine->getWindowContext(MAIN_WINDOW_ATTACHMENT_NAME).swapchain->recreate();
        renderContext->renderPass->attach();
        renderContext->framebuffer->attach();
        currentFrame = 0;
    }

    void Application::quit() {
        vkDeviceWaitIdle(engine->getLogicalDevice().getLogicalDeviceHandle());
        pipeline = nullptr;
        repositories = nullptr;
        renderContext = nullptr;
        vertexBuffer = nullptr;
        commandPool = nullptr;
        engine = nullptr;
    }
}
