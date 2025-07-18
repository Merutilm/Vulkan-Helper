//
// Created by Merutilm on 2025-07-12.
//

#pragma once
#include "../context/Engine.hpp"
#include "../handle/EngineHandler.hpp"
#include "../manage/DescriptorSetLayoutManager.hpp"
#include "../struct/DescriptorSetLayoutBuildType.hpp"


namespace merutilm::vkh {
    class DescriptorSetLayout final : public EngineHandler {
        std::unique_ptr<DescriptorSetLayoutManager> descriptorSetLayoutManager;
        VkDescriptorSetLayout layout = nullptr;

    public:
        explicit DescriptorSetLayout(const Engine &engine, std::unique_ptr<DescriptorSetLayoutManager> &&layoutManager);

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
