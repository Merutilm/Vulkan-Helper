#include "../manage/PipelineLayoutManager.hpp"
//
// Created by Merutilm on 2025-07-09.
//

#pragma once

#include "DescriptorSetLayoutBuildTypeHasher.hpp"
#include "VectorHasher.hpp"


namespace merutilm::vkh {

    struct PipelineLayoutManagerPtrHasher{
        using is_transparent = void;

        size_t operator()(const PipelineLayoutManager *key) const {
            size_t seed = 0;
            constexpr auto descHasher = VectorHasher<VkDescriptorSetLayout>();
            BoostHasher::hash(descHasher(key->getDescriptorSetLayouts()), &seed);
            constexpr auto pushMngPtrHasher = VectorHasher<PushConstantManager *>();
            BoostHasher::hash(pushMngPtrHasher(key->getPushConstantManagers()), &seed);
            return seed;
        }
    };
}
