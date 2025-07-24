//
// Created by Merutilm on 2025-07-12.
//

#pragma once
#include "Core.hpp"
#include "../handle/CoreHandler.hpp"
#include "../manage/DescriptorSetLayoutManager.hpp"


namespace merutilm::vkh {
    class DescriptorSetLayout final : public CoreHandler {
        const DescriptorSetLayoutManager & descriptorSetLayoutManager;
        VkDescriptorSetLayout layout = nullptr;

    public:
        explicit DescriptorSetLayout(const Core &core, const DescriptorSetLayoutManager &layoutManager);

        ~DescriptorSetLayout() override;

        DescriptorSetLayout(const DescriptorSetLayout &) = delete;

        DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

        DescriptorSetLayout(DescriptorSetLayout &&) = delete;

        DescriptorSetLayout &operator=(DescriptorSetLayout &&) = delete;

        [[nodiscard]] VkDescriptorSetLayout getLayoutHandle() const;

    private:
        void attach() override;

        void detach() override;
    };
}
