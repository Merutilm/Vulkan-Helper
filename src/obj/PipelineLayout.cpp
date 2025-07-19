//
// Created by Merutilm on 2025-07-13.
//

#include "PipelineLayout.hpp"

namespace merutilm::vkh {
    PipelineLayout::PipelineLayout(const Core &core, std::unique_ptr<PipelineLayoutManager> &&pipelineLayoutManager) : CoreHandler(
            core), pipelineLayoutManager(std::move(pipelineLayoutManager)) {
        PipelineLayout::attach();
    }

    PipelineLayout::~PipelineLayout() {
        PipelineLayout::detach();
    }

    void PipelineLayout::push(const VkCommandBuffer commandBuffer) const {
        uint32_t sizeSum = 0;
        for (auto &pushConstant: pipelineLayoutManager->getPushConstantManagers()) {
            const uint32_t size = pushConstant->getSizeByte();
            vkCmdPushConstants(commandBuffer, layout, pushConstant->getUseStage(),
                               sizeSum, size,
                               pushConstant->getData().data());
            sizeSum += size;
        }
    }


    void PipelineLayout::attach() {
        uint32_t sizeSum = 0;
        std::vector<VkPushConstantRange> pushConstantRanges = {};
        for (const auto &pushConstantManager : pipelineLayoutManager->getPushConstantManagers()) {
            const uint32_t size = pushConstantManager->getSizeByte();
            pushConstantRanges.emplace_back(pushConstantManager->getUseStage(), sizeSum, size);
            sizeSum += size;
        }

        const auto &descriptorSetLayouts =  pipelineLayoutManager->getDescriptorSetLayouts();

        const VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size()),
            .pSetLayouts = descriptorSetLayouts.empty() ? nullptr : descriptorSetLayouts.data(),
            .pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size()),
            .pPushConstantRanges = pushConstantRanges.empty() ? nullptr : pushConstantRanges.data(),
        };


        if (vkCreatePipelineLayout(core.getLogicalDevice().getLogicalDeviceHandle(), &pipelineLayoutInfo, nullptr,
                                   &layout) !=
            VK_SUCCESS) {
            throw exception_init("Failed to create pipeline layout!");
        }
    }


    void PipelineLayout::detach() {
        vkDestroyPipelineLayout(core.getLogicalDevice().getLogicalDeviceHandle(), layout, nullptr);
    }
}
