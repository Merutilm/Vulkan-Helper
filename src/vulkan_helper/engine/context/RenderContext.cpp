//
// Created by Merutilm on 2026-02-04.
//
#include <vulkan_helper/engine/context/RenderContext.hpp>
namespace merutilm::vkh {


    RenderContext::RenderContext(Core &core, std::function<VkExtent2D()> &&extentGetter, std::unique_ptr<RenderContextConfigurator> &&renderContextConfigurator) :
        core(core), extentGetter(std::move(extentGetter)) {
        RenderPassManager rpm{};
        const VkExtent2D extent = extentGetter();
        renderContextConfigurator->configure(rpm);
        RenderPass renderPass(core, std::move(rpm));
        Framebuffer framebuffer(core, renderPass, extent);
        this->renderContextConfigurator = std::move(renderContextConfigurator);
        this->renderPass.emplace(std::move(renderPass));
        this->framebuffer.emplace(std::move(framebuffer));
    }

    void RenderContext::recreate() {
        framebuffer.reset();
        renderPass.reset();
        RenderPassManager renderPassManager{};
        renderContextConfigurator->configure(renderPassManager);
        renderPass.emplace(core, std::move(renderPassManager));
        framebuffer.emplace(core, *renderPass, extentGetter());
    }

}