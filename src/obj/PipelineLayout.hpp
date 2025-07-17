//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include "../manage/PushConstant.hpp"
#include "../handle/EngineHandler.hpp"

namespace merutilm::vkh {
    class PipelineLayout final : public EngineHandler {
        const std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        const std::vector<PushConstant *>pushConstantManagers;
        VkPipelineLayout layout = nullptr;

    public:
        explicit PipelineLayout(const Engine &engine, const std::vector<VkDescriptorSetLayout> &descriptorSetLayout = {}, const std::vector<PushConstant *> &pushConstantManager = {});

        ~PipelineLayout() override;

        PipelineLayout(const PipelineLayout &) = delete;

        PipelineLayout &operator=(const PipelineLayout &) = delete;

        PipelineLayout(PipelineLayout &&) = delete;

        PipelineLayout &operator=(PipelineLayout &&) = delete;

        void push(VkCommandBuffer commandBuffer) const;

        [[nodiscard]] VkPipelineLayout getLayoutHandle() const { return layout ; }

    private:
        void attach() override;

        void detach() override;
    };
}
