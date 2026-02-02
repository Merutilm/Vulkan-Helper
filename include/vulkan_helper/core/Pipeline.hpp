//
// Created by Merutilm on 2025-07-11.
//

#pragma once

#include <vulkan_helper/manage/PipelineManager.hpp>
#include <vulkan_helper/handle/WindowContextHandler.hpp>

namespace merutilm::vkh {
    struct PipelineAbstract : public WindowContextHandler {
        VkPipeline pipeline = nullptr;
        PipelineLayoutRef pipelineLayout;
        const std::vector<DescriptorPtr> descriptors;
        const std::vector<ShaderModulePtr> shaderModules;

        explicit PipelineAbstract(WindowContextRef wc, PipelineLayoutRef pipelineLayout,
                                  PipelineManager &&pipelineManager);

        ~PipelineAbstract() override;

        PipelineAbstract(const PipelineAbstract &) = delete;

        PipelineAbstract &operator=(const PipelineAbstract &) = delete;

        PipelineAbstract(PipelineAbstract &&) = delete;

        PipelineAbstract &operator=(PipelineAbstract &&) = delete;

        virtual void cmdBindAll(VkCommandBuffer cbh, uint32_t frameIndex, DescIndexPicker &&descIndices) const = 0;


        [[nodiscard]] VkPipeline getPipelineHandle() const { return pipeline; }

        [[nodiscard]] DescriptorRef getDescriptor(const uint32_t setIndex) const {
            return *descriptors[setIndex];
        }

        [[nodiscard]] PipelineLayoutRef getLayout() const { return pipelineLayout; }

        [[nodiscard]] std::span<const DescriptorPtr> getDescriptors() const { return descriptors; }

        [[nodiscard]] std::span<const ShaderModulePtr> getShaderModules() const {
            return shaderModules;
        }


        [[nodiscard]] std::vector<VkDescriptorSet> enumerateDescriptorSets(const uint32_t frameIndex, DescIndexPicker &&descIndices = {}) const;

    protected:
        void destroy() override;
    };

    using Pipeline = std::unique_ptr<PipelineAbstract>;
    using PipelinePtr = PipelineAbstract *;
    using PipelineRef = PipelineAbstract &;
}
