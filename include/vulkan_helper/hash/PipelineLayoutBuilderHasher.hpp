
//
// Created by Merutilm on 2025-07-09.
//

#pragma once

#include <vulkan_helper/manage/PipelineLayoutManager.hpp>
#include <vulkan_helper/hash/UniquePtrHasher.hpp>
#include <vulkan_helper/hash/VariantHasher.hpp>
#include <vulkan_helper/hash/VectorHasher.hpp>


namespace merutilm::vkh {

    struct PipelineLayoutBuilderHasher{
        using is_transparent = void;

        size_t operator()(const PipelineLayoutBuilder &key) const {
            size_t seed = 0;
            constexpr auto descHasher = VectorHasher<PipelineLayoutBuildType, VariantHasher>();
            BoostHasher::hash(descHasher(key), &seed);
            return seed;
        }
    };
}
