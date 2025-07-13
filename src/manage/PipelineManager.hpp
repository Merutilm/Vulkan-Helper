//
// Created by Merutilm on 2025-07-11.
//

#pragma once
#include "../obj/PipelineLayout.hpp"
#include "../obj/ShaderModule.hpp"

namespace merutilm::vkh {
    class PipelineManager {
        const PipelineLayout &layout;
        std::unique_ptr<Descriptor> descriptor = nullptr;
        std::vector<std::unique_ptr<ShaderModule> > shaderModules;

    public:
        explicit PipelineManager(const PipelineLayout &layoutManager,
                                 std::unique_ptr<Descriptor> &&descriptor) : layout(layoutManager),
                                                                             descriptor(std::move(descriptor)) {
        }

        void attachShader(std::unique_ptr<ShaderModule> &&shaderStage);

        [[nodiscard]] const Descriptor &getDescriptor() const { return *descriptor; }

        [[nodiscard]] const PipelineLayout &getLayoutManager() const { return layout; }

        [[nodiscard]] const std::vector<std::unique_ptr<ShaderModule> > &getShaderModules() const {
            return shaderModules;
        }
    };

    inline void PipelineManager::attachShader(std::unique_ptr<ShaderModule> &&shaderStage) {
        shaderModules.emplace_back(std::move(shaderStage));
    }
}
