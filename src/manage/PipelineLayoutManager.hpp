//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <vector>
#include "PushConstantManager.hpp"
#include "../struct/EnumHasher.hpp"

namespace merutilm::vkh {
    class PipelineLayoutManager {

        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > pcmNames = {};
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {};
        std::vector<std::unique_ptr<PushConstantManager> > pushConstantManagers = {};

    public:
        PipelineLayoutManager() = default;

        ~PipelineLayoutManager() = default;

        PipelineLayoutManager(const PipelineLayoutManager &) = delete;

        PipelineLayoutManager &operator=(const PipelineLayoutManager &) = delete;

        PipelineLayoutManager(PipelineLayoutManager &&) = delete;

        PipelineLayoutManager &operator=(PipelineLayoutManager &&) = delete;

        bool operator==(const PipelineLayoutManager &) const = default;

        void appendDescriptorSetLayout(const VkDescriptorSetLayout descriptorSetLayout) {
            descriptorSetLayouts.emplace_back(descriptorSetLayout);
        }

        void appendPushConstantManager(const std::string &pushName, std::unique_ptr<PushConstantManager> &&pushConstantManager) {
            if (pcmNames.contains(pushName)) {
                throw exception_invalid_args("Duplicate push name : " + pushName);
            }
            pcmNames[pushName] = pushConstantManagers.size();
            pushConstantManagers.emplace_back(std::move(pushConstantManager));
        }

        std::span<const VkDescriptorSetLayout> getDescriptorSetLayouts() const {
            return descriptorSetLayouts;
        }

        std::span<const std::unique_ptr<PushConstantManager>> getPushConstantManagers() const {
            return pushConstantManagers;
        }

        PushConstantManager &getPushConstantManager(const std::string &pushName) const {
            if (!pcmNames.contains(pushName)) {
                throw exception_invalid_args("Not found push name : " + pushName);
            }
            return *pushConstantManagers[pcmNames.at(pushName)];
        }
    };
}
