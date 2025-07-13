//
// Created by Merutilm on 2025-07-13.
//

#include "PipelineLayout.hpp"

namespace merutilm::vkh {
    PipelineLayout::PipelineLayout(const Engine &engine, const Descriptor &descriptor,
                                   const PushConstantManager &pushConstantManager) : EngineHandler(engine), descriptor(descriptor),
        pushConstantManager(pushConstantManager) {
        PipelineLayout::init();
    }

    PipelineLayout::~PipelineLayout() {
        PipelineLayout::destroy();
    }

    void PipelineLayout::push(const CommandBuffer &commandBuffer, const uint32_t frameIndex) const {
        vkCmdPushConstants(commandBuffer.getCommandBufferHandle(frameIndex), layout, pushConstantManager.useStage,
                           0, pushConstantManager.manager->getSize(),
                           pushConstantManager.manager->getData().data());
    }


    void PipelineLayout::init() {
        const DescriptorManager &descriptorManager = descriptor.getDescriptorManager();
        const DescriptorSetLayout &descriptorSetLayout = descriptorManager.getLayout();
        VkDescriptorSetLayout descriptorSetLayoutHandle = descriptorSetLayout.getLayoutHandle();

        VkPushConstantRange pushConstantRange = {
            .stageFlags = pushConstantManager.useStage,
            .offset = 0,
            .size = pushConstantManager.manager->getSize(),
        };


        const VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = 1,
            .pSetLayouts = &descriptorSetLayoutHandle,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &pushConstantRange
        };

        if (vkCreatePipelineLayout(engine.getLogicalDevice().getLogicalDeviceHandle(), &pipelineLayoutInfo, nullptr, &layout) !=
            VK_SUCCESS) {
            throw exception_init("Failed to create pipeline layout!");
        }
    }


    void PipelineLayout::destroy() {
        vkDestroyPipelineLayout(engine.getLogicalDevice().getLogicalDeviceHandle(), layout, nullptr);
    }
}
