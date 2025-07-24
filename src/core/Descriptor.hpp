//
// Created by Merutilm on 2025-07-09.
//

#pragma once

#include "DescriptorSetLayout.hpp"
#include "../manage/DescriptorManager.hpp"

namespace merutilm::vkh {
    class Descriptor final : public CoreHandler {

        std::vector<VkDescriptorPool> descriptorPools = {};
        std::vector<VkDescriptorSet> descriptorSets = {};
        const DescriptorSetLayout &descriptorSetLayout;
        const std::unique_ptr<DescriptorManager> descriptorManager = nullptr;

    public:
        explicit Descriptor(const Core &core, const DescriptorSetLayout &descriptorSetLayout, std::unique_ptr<DescriptorManager> &&descriptorManager);

        ~Descriptor() override;

        Descriptor(const Descriptor &) = delete;

        Descriptor &operator=(const Descriptor &) = delete;

        Descriptor(Descriptor &&) = delete;

        Descriptor &operator=(Descriptor &&) = delete;

        [[nodiscard]] DescriptorManager &getDescriptorManager() const { return *descriptorManager; }

        [[nodiscard]] VkDescriptorSet getDescriptorSetHandle(const uint32_t frameIndex) const { return descriptorSets[frameIndex]; }

        [[nodiscard]] VkDescriptorPool getDescriptorPoolHandle(const uint32_t frameIndex) const { return descriptorPools[frameIndex]; }

        void update(uint32_t frameIndex) const;

    private:
        void attach() override;

        void detach() override;
    };
}
