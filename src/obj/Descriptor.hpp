//
// Created by Merutilm on 2025-07-09.
//

#pragma once

#include "../manage/DescriptorManager.hpp"

namespace merutilm::vkh {
    class Descriptor final : public EngineHandler {
        std::vector<VkDescriptorPool> descriptorPools = {};
        std::vector<VkDescriptorSet> descriptorSets = {};
        const std::unique_ptr<DescriptorManager> descriptorManager = nullptr;

    public:
        explicit Descriptor(const Engine &engine, std::unique_ptr<DescriptorManager> &&descriptorManager);

        ~Descriptor() override;

        Descriptor(const Descriptor &) = delete;

        Descriptor &operator=(const Descriptor &) = delete;

        Descriptor(Descriptor &&) = delete;

        Descriptor &operator=(Descriptor &&) = delete;

        [[nodiscard]] const DescriptorManager &getDescriptorManager() const { return *descriptorManager; };

        void update(uint32_t frameIndex) const;

    private:
        void init() override;

        void destroy() override;
    };
}
