//
// Created by Merutilm on 2025-07-18.
//

#pragma once

#include <vulkan_helper/base/pch.hpp>
#include <vulkan_helper/engine/cmd/CommandBuffer.hpp>
#include <vulkan_helper/engine/repo/Repositories.hpp>
#include <vulkan_helper/engine/sync/SyncObject.hpp>
#include <vulkan_helper/engine/window/Surface.hpp>
#include <vulkan_helper/engine/window/Swapchain.hpp>
#include "RenderContext.hpp"
#include "SharedImageContext.hpp"

namespace merutilm::vkh {

    class Engine;

    class WindowContext final : public CoreHandler {
        const uint32_t attachmentIndex;
        CommandPool &commandPool;
        std::unique_ptr<PlatformWindow> window;
        std::optional<Surface> surface;
        std::optional<Swapchain> swapchain;
        std::optional<Repositories> windowLocalRepositories;
        std::optional<CommandBuffer> commandBuffer;
        std::optional<SyncObject> syncObject;
        std::optional<SharedImageContext> sharedImageContext;
        std::vector<std::unique_ptr<RenderContext>> renderContexts = {};

    public:
        explicit WindowContext(Core &core, CommandPool &commandPool, uint32_t index, std::unique_ptr<PlatformWindow> &&window);

        ~WindowContext() override;

        WindowContext(const WindowContext &) = delete;

        WindowContext operator=(const WindowContext &) = delete;

        WindowContext(WindowContext &&) = delete;

        WindowContext operator=(WindowContext &&) = delete;

        template<typename T, typename ExtentImgGetter, typename SwapchainImgGetter>
            requires(std::is_base_of_v<RenderPassGraphGeneratorBase, T> &&
                     std::is_invocable_r_v<VkExtent2D, ExtentImgGetter> &&
                     std::is_invocable_r_v<MultiframeImageContext, SwapchainImgGetter>)
        RenderContext *attachRenderContext(Engine &engine, ExtentImgGetter &&extentGetter,
                                 SwapchainImgGetter &&swapchainImageGetter) {
            return this->renderContexts.emplace_back(std::make_unique<RenderContext>(
                    core, std::forward<ExtentImgGetter>(extentGetter),
                    std::make_unique<T>(engine, *this,
                                        std::forward<SwapchainImgGetter>(swapchainImageGetter)))).get();
        }

        [[nodiscard]] uint32_t getAttachmentIndex() const { return attachmentIndex; }

        [[nodiscard]] PlatformWindow *getWindow() const { return window.get(); }

        [[nodiscard]] Surface &getSurface() { return *surface; }

        [[nodiscard]] Swapchain &getSwapchain() { return *swapchain; }

        [[nodiscard]] Repositories &getWindowLocalRepositories() { return *windowLocalRepositories; }

        [[nodiscard]] CommandBuffer &getCommandBuffer() { return *commandBuffer; }

        [[nodiscard]] SyncObject &getSyncObject() { return *syncObject; }

        [[nodiscard]] SharedImageContext &getSharedImageContext() { return *sharedImageContext; }

        [[nodiscard]] std::span<const std::unique_ptr<RenderContext>> getRenderContexts() const { return renderContexts; }

        [[nodiscard]] RenderContext &getRenderContext(const uint32_t renderContextIndex) const {
            return *renderContexts[renderContextIndex];
        }


    protected:
        void init() override;

        void cleanup() override;

    private:

        void configureRepositories();
    };

} // namespace merutilm::vkh
