//
// Created by Merutilm on 2025-07-12.
//

#pragma once
#include "Engine.hpp"
#include "../objhandler/EngineHandler.hpp"
#include "../struct/DescriptorLayoutBuildType.hpp"


namespace merutilm::vkh {
    class DescriptorSetLayout final : public EngineHandler {
        std::vector<DescriptorLayoutBuildType> layoutBuilder = {};
        VkDescriptorSetLayout layout = nullptr;

    public:
        explicit DescriptorSetLayout(const Engine &engine);

        ~DescriptorSetLayout() override;

        DescriptorSetLayout(const DescriptorSetLayout &) = delete;

        DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

        DescriptorSetLayout(DescriptorSetLayout &&) = delete;

        DescriptorSetLayout &operator=(DescriptorSetLayout &&) = delete;

        void append(DescriptorLayoutBuildType buildType);

        void finish();

        [[nodiscard]] uint32_t getDescriptorCount() const { return static_cast<uint32_t>(layoutBuilder.size()); }

        [[nodiscard]] VkDescriptorSetLayout getLayoutHandle() const;

        [[nodiscard]] const DescriptorLayoutBuildType &getElement(const uint32_t index) const {
            return layoutBuilder[index];
        }

    private:
        void init() override;

        void destroy() override;
    };
}
