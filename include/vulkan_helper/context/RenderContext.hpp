//
// Created by Merutilm on 2025-07-18.
//

#pragma once

#include <vulkan_helper/configurator/RenderContextConfigurator.hpp>
#include <vulkan_helper/core/Framebuffer.hpp>
#include <vulkan_helper/core/RenderPass.hpp>

namespace merutilm::vkh {
    class RenderContextImpl final {
        CoreRef core;
        RenderContextConfigurator renderContextConfigurator = nullptr;
        RenderPass renderPass = nullptr;
        Framebuffer framebuffer = nullptr;
        std::function<VkExtent2D()> extentGetter;

    public:
        explicit RenderContextImpl(CoreRef core, std::function<VkExtent2D()> &&extentGetter,
                                   RenderContextConfigurator &&renderPassConfigurator);

        ~RenderContextImpl();

        RenderContextImpl(const RenderContextImpl &) = delete;

        RenderContextImpl &operator=(const RenderContextImpl &) = delete;

        RenderContextImpl(RenderContextImpl &&) = delete;

        RenderContextImpl &operator=(RenderContextImpl &&) = delete;

        void recreate();

        [[nodiscard]] RenderContextConfiguratorPtr getConfigurator() const {
            return renderContextConfigurator.get();
        }

        [[nodiscard]] RenderPassPtr getRenderPass() const {
            return renderPass.get();
        }

        [[nodiscard]] FramebufferPtr getFramebuffer() const {
            return framebuffer.get();
        }
    };

    using RenderContext = std::unique_ptr<RenderContextImpl>;
    using RenderContextPtr = RenderContextImpl *;
    using RenderContextRef = RenderContextImpl &;
}
