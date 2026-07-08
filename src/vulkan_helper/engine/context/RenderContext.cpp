//
// Created by Merutilm on 2026-02-04.
//
#include <vulkan_helper/engine/context/RenderContext.hpp>
namespace merutilm::vkh {


    RenderContext::RenderContext(Core &core, std::function<VkExtent2D()> &&extentGetter, std::unique_ptr<RenderPassGraphGeneratorBase> &&renderPassGraphGenerator) :
        core(core), extentGetter(std::move(extentGetter)) {
        const VkExtent2D extent = this->extentGetter();
        this->renderPassGraphGenerator = std::move(renderPassGraphGenerator);
        this->renderPass.emplace(core, this->renderPassGraphGenerator->regenerate());
        this->renderPassGraphGenerator->createPipelines(&*this->renderPass);
        this->framebuffer.emplace(core, *renderPass, extent);
    }

    void RenderContext::recreate() {
        framebuffer.reset();
        renderPass.reset();
        renderPass.emplace(core, renderPassGraphGenerator->regenerate());
        framebuffer.emplace(core, *renderPass, extentGetter());
    }

}