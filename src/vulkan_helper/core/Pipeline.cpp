//
// Created by Merutilm on 2026-02-03.
//
#include "../../../include/vulkan_helper/core/Pipeline.hpp"

namespace merutilm::vkh {
    PipelineAbstract::PipelineAbstract(WindowContextRef wc, PipelineLayoutRef pipelineLayout,
        PipelineManager &&pipelineManager): WindowContextHandler(wc),
                                            pipelineLayout(pipelineLayout),
                                            descriptors(
                                                std::move(pipelineManager->descriptors)),
                                            shaderModules(
                                                std::move(
                                                    pipelineManager->shaderModules)) {
    }

    PipelineAbstract::~PipelineAbstract() = default;

    std::vector<VkDescriptorSet> PipelineAbstract::enumerateDescriptorSets(const uint32_t frameIndex,
        DescIndexPicker &&descIndices) const {
        std::vector<VkDescriptorSet> sets(descriptors.size());

        if (!descIndices.empty()) {
            safe_array::check_size_equal(descriptors.size(), descIndices.size(), "Descriptor Index");
        }else {
            descIndices = std::vector<uint32_t>(descriptors.size(), 0);
        }

        for (uint32_t i = 0; i < descriptors.size(); i++) {
            sets[i] = descriptors[i]->getDescriptorSetHandle(frameIndex, descIndices[i]);
        }
        return sets;
    }

    void PipelineAbstract::destroy() {
        allocator::invoke(vkDestroyPipeline, wc.core.getLogicalDevice().getLogicalDeviceHandle(), pipeline, nullptr);
    }
}
