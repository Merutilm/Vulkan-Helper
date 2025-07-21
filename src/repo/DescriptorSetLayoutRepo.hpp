//
// Created by Merutilm on 2025-07-16.
//

#pragma once
#include <memory>
#include <unordered_map>

#include "Repo.hpp"
#include "../manage/DescriptorSetLayoutManager.hpp"
#include "../obj/DescriptorSetLayout.hpp"
#include "../struct/DescriptorSetLayoutName.hpp"
#include "../struct/EnumHasher.hpp"
#include "../template/DSLTemplate.hpp"

namespace merutilm::vkh {
    struct DescriptorSetLayoutRepo final : Repo<DescriptorSetLayoutName, DescriptorSetLayoutName, DescriptorSetLayout,
                EnumHasher, std::equal_to<>> {
        using Repo::Repo;

        const DescriptorSetLayout &pick(const DescriptorSetLayoutName layoutName) override {
            return *repository.try_emplace(
                        layoutName, std::make_unique<DescriptorSetLayout>(core, findDescriptorSetLayoutManager(layoutName))).
                    first->
                    second;
        }

        static std::unique_ptr<DescriptorSetLayoutManager> findDescriptorSetLayoutManager(const DescriptorSetLayoutName layoutName) {
            switch (layoutName) {
                using enum DescriptorSetLayoutName;
                case UNIFORM_V1: {
                    return DSLTemplate::uniform_v1();
                }
                case UNIFORM_F1: {
                    return DSLTemplate::uniform_f1();
                }
                case UNIFORM_VF1 : {
                    return DSLTemplate::uniform_vf1();
                }
            }
            throw exception_invalid_args("invalid descriptor set layout name");
        }
    };
}
