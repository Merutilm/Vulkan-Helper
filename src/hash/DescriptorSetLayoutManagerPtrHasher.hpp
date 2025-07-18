//
// Created by Merutilm on 2025-07-09.
//

#pragma once

#include "DescriptorSetLayoutBuildTypeHasher.hpp"
#include "VectorHasher.hpp"
#include "../manage/DescriptorSetLayoutManager.hpp"


namespace merutilm::vkh {

    struct DescriptorSetLayoutManagerPtrHasher{
        using is_transparent = void;

        size_t operator()(const DescriptorSetLayoutManager *key) const {
            constexpr auto hasher = VectorHasher<DescriptorSetLayoutBuildType, DescriptorSetLayoutBuildTypeHasher>();
            return hasher(key->getLayoutBuildTypes());
        }
    };
}
