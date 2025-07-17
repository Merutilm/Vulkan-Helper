//
// Created by Merutilm on 2025-07-11.
//

#pragma once

#include "../obj/Descriptor.hpp"
#include "../obj/PipelineLayout.hpp"
#include "../obj/ShaderModule.hpp"

namespace merutilm::vkh {
    class PipelineManager {
        const PipelineLayout &layout;
        std::unique_ptr<Descriptor> descriptor = nullptr;
        std::vector<const ShaderModule *> shaderModules;

    public:
        explicit PipelineManager(const PipelineLayout &layoutManager,
                                 std::unique_ptr<Descriptor> &&descriptor) : layout(layoutManager),
                                                                             descriptor(std::move(descriptor)) {
        }

        void attachShader(const ShaderModule *shaderStage);

        [[nodiscard]] const Descriptor &getDescriptor() const { return *descriptor; }

        [[nodiscard]] Descriptor &getDescriptor() { return *descriptor; }

        [[nodiscard]] const PipelineLayout &getLayoutManager() const { return layout; }


        [[nodiscard]] std::span<const ShaderModule * const> getShaderModules() const {
            return shaderModules;
        }
    };

    inline void PipelineManager::attachShader(const ShaderModule *shaderStage) {
        shaderModules.emplace_back(shaderStage);
    }
}
