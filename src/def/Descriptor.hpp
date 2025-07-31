//
// Created by Merutilm on 2025-07-09.
//

#pragma once

#include <algorithm>
#include <numeric>

#include "DescriptorSetLayout.hpp"
#include "../context/ImageContext.hpp"
#include "../manage/DescriptorManager.hpp"

namespace merutilm::vkh {
    class Descriptor final : public CoreHandler {
        std::vector<VkDescriptorPool> descriptorPools = {};
        std::vector<VkDescriptorSet> descriptorSets = {};
        const DescriptorSetLayout &descriptorSetLayout;
        const std::unique_ptr<DescriptorManager> descriptorManager = nullptr;

    public:
        explicit Descriptor(const Core &core, const DescriptorSetLayout &descriptorSetLayout,
                            std::unique_ptr<DescriptorManager> &&descriptorManager);

        ~Descriptor() override;

        Descriptor(const Descriptor &) = delete;

        Descriptor &operator=(const Descriptor &) = delete;

        Descriptor(Descriptor &&) = delete;

        Descriptor &operator=(Descriptor &&) = delete;

        [[nodiscard]] DescriptorManager &getDescriptorManager() const { return *descriptorManager; }

        [[nodiscard]] VkDescriptorSet getDescriptorSetHandle(const uint32_t frameIndex) const {
            return descriptorSets[frameIndex];
        }

        [[nodiscard]] VkDescriptorPool getDescriptorPoolHandle(const uint32_t frameIndex) const {
            return descriptorPools[frameIndex];
        }

        void update(uint32_t frameIndex, uint32_t imageIndex) const;

        template<typename... T>
        void update(uint32_t frameIndex, uint32_t imageIndex, T &&... varname) const;

        void updateIndices(uint32_t frameIndex, const std::vector<uint32_t> &indices) const;

    private:
        void attach() override;

        void detach() override;
    };


    inline void Descriptor::update(const uint32_t frameIndex, const uint32_t imageIndex) const {
        const uint32_t elementCount = descriptorManager->getElements();
        std::vector<uint32_t> specifiedIndices(elementCount);
        std::iota(specifiedIndices.begin(), specifiedIndices.end(), 0);
        updateIndices(frameIndex, specifiedIndices);
    }


    template<typename... T>
    void Descriptor::update(const uint32_t frameIndex, const uint32_t imageIndex, T &&... varname) const {
        std::vector<uint32_t> specifiedIndices;
        (specifiedIndices.push_back(descriptorManager->getIndex(std::forward<T>(varname))), ...);
        std::ranges::sort(specifiedIndices);
        updateIndices(frameIndex, specifiedIndices);
    }
}
