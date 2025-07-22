//
// Created by Merutilm on 2025-07-12.
//

#pragma once
#include "Core.hpp"
#include "../handle/CoreHandler.hpp"
#include "../manage/DescriptorSetLayoutManager.hpp"
#include "../struct/DescriptorSetLayoutName.hpp"


namespace merutilm::vkh {
    class DescriptorSetLayout final : public CoreHandler {
        std::unique_ptr<DescriptorSetLayoutManager> descriptorSetLayoutManager;
        VkDescriptorSetLayout layout = nullptr;

    public:
        explicit DescriptorSetLayout(const Core &core, std::unique_ptr<DescriptorSetLayoutManager> &&layoutManager);

        ~DescriptorSetLayout() override;

        DescriptorSetLayout(const DescriptorSetLayout &) = delete;

        DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

        DescriptorSetLayout(DescriptorSetLayout &&) = delete;

        DescriptorSetLayout &operator=(DescriptorSetLayout &&) = delete;

        [[nodiscard]] VkDescriptorSetLayout getLayoutHandle() const;

        [[nodiscard]] const DescriptorSetLayoutManager &getDescriptorSetLayoutManager() const {return *descriptorSetLayoutManager;}

    private:
        void attach() override;

        void detach() override;
    };
}
