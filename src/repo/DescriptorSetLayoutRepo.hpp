//
// Created by Merutilm on 2025-07-16.
//

#pragma once
#include <memory>
#include <unordered_map>

#include "Repo.hpp"
#include "../manage/DescriptorSetLayoutManager.hpp"
#include "../core/DescriptorSetLayout.hpp"
#include "../hash/DescriptorSetLayoutManagerHasher.hpp"

namespace merutilm::vkh {
    struct DescriptorSetLayoutRepo final : Repo<const DescriptorSetLayoutManager *, const DescriptorSetLayoutManager &, DescriptorSetLayout,
                DescriptorSetLayoutManagerHasher, std::equal_to<>> {
        using Repo::Repo;

        const DescriptorSetLayout &pick(const DescriptorSetLayoutManager &layoutManager) override {
            const auto ptr = &layoutManager;
            return *repository.try_emplace(
                        ptr, std::make_unique<DescriptorSetLayout>(core, layoutManager)).
                    first->
                    second;
        }

    };
}
