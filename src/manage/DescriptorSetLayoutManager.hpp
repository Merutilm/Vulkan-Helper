//
// Created by Merutilm on 2025-07-17.
//

#pragma once

#include <vector>
#include "../struct/DescriptorSetLayoutBuildType.hpp"


namespace merutilm::vkh {
    class DescriptorSetLayoutManager {
        std::vector<DescriptorSetLayoutBuildType> layoutBuildTypes = {};

    public:
        explicit DescriptorSetLayoutManager() = default;

        ~DescriptorSetLayoutManager() = default;

        DescriptorSetLayoutManager(const DescriptorSetLayoutManager &) = delete;

        DescriptorSetLayoutManager &operator=(const DescriptorSetLayoutManager &) = delete;

        DescriptorSetLayoutManager(DescriptorSetLayoutManager &&) = delete;

        DescriptorSetLayoutManager &operator=(DescriptorSetLayoutManager &&) = delete;

        void append(const DescriptorSetLayoutBuildType buildType) { layoutBuildTypes.push_back(buildType); }

        bool operator==(const DescriptorSetLayoutManager &other) const = default;

        [[nodiscard]] const std::vector<DescriptorSetLayoutBuildType> &getLayoutBuildTypes() const { return layoutBuildTypes; }

        [[nodiscard]] uint32_t getDescriptorCount() const { return static_cast<uint32_t>(layoutBuildTypes.size()); }

        [[nodiscard]] const DescriptorSetLayoutBuildType &getElement(const uint32_t index) const {
            return layoutBuildTypes[index];
        }
    };
}
