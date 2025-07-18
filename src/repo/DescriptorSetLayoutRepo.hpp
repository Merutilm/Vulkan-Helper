//
// Created by Merutilm on 2025-07-16.
//

#pragma once
#include <memory>
#include <unordered_map>

#include "Repo.hpp"
#include "../manage/DescriptorSetLayoutManager.hpp"
#include "../obj/DescriptorSetLayout.hpp"
#include "../hash/DescriptorSetLayoutManagerPtrHasher.hpp"
#include "../hash/PointerDerefEquals.hpp"

namespace merutilm::vkh {
    struct DescriptorSetLayoutRepo final : Repo<DescriptorSetLayoutManager *, DescriptorSetLayout, DescriptorSetLayoutManagerPtrHasher, PointerDerefEquals, std::unique_ptr<DescriptorSetLayoutManager> &&>{

        using Repo::Repo;

        const DescriptorSetLayout &pick(std::unique_ptr<DescriptorSetLayoutManager> &&layoutManager) override {

            auto *ptr = layoutManager.get();
            return *repository.try_emplace(
                        ptr, std::make_unique<DescriptorSetLayout>(engine, std::move(layoutManager))).first->
                    second;
        }

    };
}
