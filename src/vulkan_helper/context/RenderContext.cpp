//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/context/RenderContext.hpp>
namespace merutilm::vkh {
    RenderContextImpl::RenderContextImpl(CoreRef core, std::function<VkExtent2D()> &&extentGetter,
        RenderContextConfigurator &&renderPassConfigurator): core(core),
                                                             extentGetter(std::move(extentGetter)) {
        auto renderPassManager = factory::create<RenderPassManager>();
        renderContextConfigurator = std::move(renderPassConfigurator);
        const VkExtent2D extent = this->extentGetter();
        renderContextConfigurator->configure(*renderPassManager);
        renderPass = factory::create<RenderPass>(core, std::move(renderPassManager));
        framebuffer = factory::create<Framebuffer>(core, *renderPass, extent);
    }

    RenderContextImpl::~RenderContextImpl() = default;

    void RenderContextImpl::recreate() {
        framebuffer = nullptr;
        renderPass = nullptr;
        VkExtent2D extent = extentGetter();
        auto renderPassManager = factory::create<RenderPassManager>();
        renderContextConfigurator->configure(*renderPassManager);
        renderPass = factory::create<RenderPass>(core, std::move(renderPassManager));
        framebuffer = factory::create<Framebuffer>(core, *renderPass, extent);
    }
}
