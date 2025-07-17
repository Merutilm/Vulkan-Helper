//
// Created by Merutilm on 2025-07-07.
//

#include "Application.hpp"

#include <iostream>
#include <glm/glm.hpp>

#include "UniformBuffer.hpp"
#include "../executor/CommandExecutor.hpp"
#include "../executor/RenderPassExecutor.hpp"
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
        createEngine();
        syncObject = std::make_unique<SyncObject>(*engine);
        commandPool = std::make_unique<CommandPool>(*engine);
        commandBuffer = std::make_unique<CommandBuffer>(*engine, *commandPool);
        createRenderPass();


        auto vertexBufferManager = std::make_unique<BufferObjectManager>();
        const std::vector<Vertex> vertices{
            {{0.0f, -0.5f, 0.0f}, {1, 0, 0}},
            {{0.5f, 0.5f, 0.0f}, {0, 1, 0}},
            {{-0.5f, 0.5f, 0.0f}, {0, 0, 1}}
        };
        vertexBufferManager->add("vertices", vertices);
        vertexBuffer = std::make_unique<VertexBuffer>(*engine, std::move(vertexBufferManager));
        framebuffer = std::make_unique<Framebuffer>(*engine, *renderPass);

        auto descriptorLayoutManager = std::make_unique<DescriptorSetLayoutManager>();
        descriptorLayoutManager->append({.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .stage = VK_SHADER_STAGE_FRAGMENT_BIT});

        descriptorLayout = std::make_unique<DescriptorSetLayout>(*engine, std::move(descriptorLayoutManager));
        pipelineLayout = std::make_unique<PipelineLayout>(*engine, std::vector{descriptorLayout->getLayoutHandle()});

        auto descManager = std::make_unique<DescriptorManager>(*descriptorLayout);

        auto uploader = std::make_unique<BufferObjectManager>();
        uploader->reserve<uint32_t>("time");

        auto ubo = std::make_unique<UniformBuffer>(*engine, std::move(uploader));
        descManager->appendUBO("SS", std::move(ubo));

        auto descriptor = std::make_unique<Descriptor>(*engine, std::move(descManager));
        auto pipelineManager = std::make_unique<PipelineManager>(*pipelineLayout, std::move(descriptor));
        auto &vert = ShaderModuleRepo::acquireShaderModule(*engine, "../shaders/template.vert");
        auto &frag = ShaderModuleRepo::acquireShaderModule(*engine, "../shaders/template.frag");
        pipelineManager->attachShader(&vert);
        pipelineManager->attachShader(&frag);
        pipeline = std::make_unique<Pipeline>(*engine, *pipelineLayout, *renderPass, *vertexBuffer, "Sample",
                                              std::move(pipelineManager));

        const HCURSOR hCursor = LoadCursor(nullptr, IDC_ARROW);


        engine->getWindow().setListener(WM_MOUSEMOVE, [hCursor](const Window &) {
            SetCursor(hCursor);
            return static_cast<LRESULT>(true);
        });
        engine->getWindow().setListener(WM_SIZE, [this](const Window &) {
            resolveResize();
            return static_cast<LRESULT>(0);
        });
        engine->getWindow().appendRenderer([this] { drawFrame(); });
        engine->getWindow().start();
    }


    void Application::createEngine() {
        EngineInitInfo engineInitInfo = {
            .windowName = L"Main",
            .width = 1280,
            .height = 720,
            .framerate = 120
        };
        engine = Engine::createEngine(std::move(engineInitInfo));
    }

    void Application::createRenderPass() {
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

        renderPass = std::make_unique<RenderPass>(*engine, std::move(renderPassManager));
    }

    void Application::drawFrame() {
        if (engine->getWindow().isUnrenderable()) {
            return;
        }
        const VkDevice device = engine->getLogicalDevice().getLogicalDeviceHandle();
        const VkFence currentFence = syncObject->getFence(currentFrame);
        const VkSemaphore imageAvailableSemaphore = syncObject->getImageAvailableSemaphore(currentFrame);
        const VkSwapchainKHR swapchainHandle = engine->getSwapchain().getSwapchainHandle();


        vkWaitForFences(device, 1, &currentFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &currentFence);

        uint32_t imageIndex = 0;
        vkAcquireNextImageKHR(device, swapchainHandle, UINT64_MAX, imageAvailableSemaphore,
                              nullptr, &imageIndex);

        //COMMAND SCOPE START
        {
            const auto executor = CommandExecutor(*engine, *commandBuffer, *renderPass, *framebuffer, *syncObject,
                                                  currentFrame, imageIndex);
            const auto renderPassExecutor = RenderPassExecutor(*engine, *commandBuffer, *renderPass, *framebuffer,
                                                               currentFrame, imageIndex);
            const VkCommandBuffer cbh = executor.getCommandBufferHandle();
            pipelineLayout->push(cbh);
            const VkDescriptorSet sets = pipeline->getPipelineManager().getDescriptor().getDescriptorSetHandle(
                imageIndex);
            vkCmdBindPipeline(cbh, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineHandle());
            vkCmdBindDescriptorSets(cbh, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pipeline->getPipelineManager().getLayoutManager().getLayoutHandle(), 0, 1, &sets, 0,
                                    nullptr);
            const auto [width, height] = engine->getSwapchain().populateSwapchainExtent();
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

            pipeline->getPipelineManager().getDescriptor().getDescriptorManager().getUBO("SS").set("time", time);
            pipeline->getPipelineManager().getDescriptor().update(currentFrame);
            vertexBuffer->update(currentFrame);

            constexpr VkDeviceSize vertexBufferOffset = 0;
            vkCmdSetViewport(cbh, 0, 1, &viewport);
            vkCmdSetScissor(cbh, 0, 1, &scissor);
            vkCmdBindVertexBuffers(cbh, 0, 1, &vertexBufferHandle, &vertexBufferOffset);
            vkCmdDraw(cbh, 3, 1, 0, 0);
        } // COMMAND SCOPE END
        changeFrameIndex();
    }

    void Application::changeFrameIndex() {
        ++currentFrame %= engine->getSwapchain().getMaxFramesInFlight();
    }

    void Application::resolveResize() {
        if (engine->getWindow().isUnrenderable()) {
            return;
        }
        vkDeviceWaitIdle(engine->getLogicalDevice().getLogicalDeviceHandle());
        framebuffer->detach();
        renderPass->detach();
        engine->getSwapchain().recreate();
        renderPass->attach();
        framebuffer->attach();
        currentFrame = 0;
    }

    void Application::quit() {
        vkDeviceWaitIdle(engine->getLogicalDevice().getLogicalDeviceHandle());
        pipeline = nullptr;
        ShaderModuleRepo::releaseAll();
        pipelineLayout = nullptr;
        descriptorLayout = nullptr;
        framebuffer = nullptr;
        vertexBuffer = nullptr;
        renderPass = nullptr;
        commandBuffer = nullptr;
        commandPool = nullptr;
        syncObject = nullptr;
        engine = nullptr;
    }
}
