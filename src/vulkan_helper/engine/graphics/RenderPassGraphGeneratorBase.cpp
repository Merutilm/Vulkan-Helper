//
// Created by Merutilm on 7/9/26.
//

#include <vulkan_helper/engine/graphics/RenderPassGraphGeneratorBase.hpp>
#include <vulkan_helper/engine/pipeline/GraphicsPipelineNode.hpp>

namespace merutilm::vkh {


    RenderPassGraphGeneratorBase::RenderPassGraphGeneratorBase() = default;

    RenderPassGraphGeneratorBase::~RenderPassGraphGeneratorBase() = default;

    void RenderPassGraphGeneratorBase::regenerate() {
        rpm.attachments.clear();
        configureAttachments();
    }

    void RenderPassGraphGeneratorBase::configureAll() {
        configureAttachments();
        configurePipelines();
        generate();
    }

} // namespace merutilm::vkh
