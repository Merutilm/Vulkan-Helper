//
// Created by Merutilm on 2025-07-11.
//

#pragma once

#include "../core/Descriptor.hpp"
#include "../core/PipelineLayout.hpp"
#include "../core/ShaderModule.hpp"

namespace merutilm::vkh {
    class PipelineManager {
        const PipelineLayout &layout;
        std::vector<const Descriptor *> descriptors = {};
        std::vector<const ShaderModule *> shaderModules;

    public:
        explicit PipelineManager(const PipelineLayout &layout) : layout(layout) {
        }

        void attachShader(const ShaderModule *shaderStage);

        void attachDescriptor(std::vector<const Descriptor *> &&descriptor) { descriptors = std::move(descriptor); }

        [[nodiscard]] const Descriptor &getDescriptor(const uint32_t setNumber) const {
            return *descriptors[setNumber];
        }

        [[nodiscard]] const Descriptor &getDescriptor(const uint32_t setNumber) { return *descriptors[setNumber]; }

        [[nodiscard]] const PipelineLayout &getLayout() const { return layout; }

        [[nodiscard]] std::span<const ShaderModule * const> getShaderModules() const {
            return shaderModules;
        }
    };

    inline void PipelineManager::attachShader(const ShaderModule *shaderStage) {
        shaderModules.emplace_back(shaderStage);
    }
}
