//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <vector>
#include "PushConstantManager.hpp"

namespace merutilm::vkh {
    class PipelineLayoutManager {
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {};
        std::vector<PushConstantManager *> pushConstantManagers = {};

    public:
        PipelineLayoutManager() = default;

        ~PipelineLayoutManager() = default;

        PipelineLayoutManager(const PipelineLayoutManager &) = delete;

        PipelineLayoutManager &operator=(const PipelineLayoutManager &) = delete;

        PipelineLayoutManager(PipelineLayoutManager &&) = delete;

        PipelineLayoutManager &operator=(PipelineLayoutManager &&) = delete;

        bool operator==(const PipelineLayoutManager &) const = default;

        void appendDescriptorSetLayout(const VkDescriptorSetLayout descriptorSetLayout) {
            descriptorSetLayouts.push_back(descriptorSetLayout);
        }

        void appendPushConstantManager(PushConstantManager *pushConstantManager) {
            pushConstantManagers.push_back(pushConstantManager);
        }

        std::span<const VkDescriptorSetLayout> getDescriptorSetLayouts() const {
            return descriptorSetLayouts;
        }

        std::span<PushConstantManager * const> getPushConstantManagers() const {
            return pushConstantManagers;
        }
    };
}
