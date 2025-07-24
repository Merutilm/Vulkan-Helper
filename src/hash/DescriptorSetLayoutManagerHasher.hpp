//
// Created by Merutilm on 2025-07-23.
//

#pragma once
#include "DescriptorSetLayoutBuildTypeHasher.hpp"
#include "VectorHasher.hpp"
#include "../manage/DescriptorSetLayoutManager.hpp"

namespace merutilm::vkh {

    struct DescriptorSetLayoutManagerHasher {
        using is_transparent = void;

        size_t operator()(const DescriptorSetLayoutManager * manager) const {
            return VectorHasher<DescriptorSetLayoutBuildType, DescriptorSetLayoutBuildTypeHasher> {}(manager->getLayoutBuildTypes());
        }
    };
}
