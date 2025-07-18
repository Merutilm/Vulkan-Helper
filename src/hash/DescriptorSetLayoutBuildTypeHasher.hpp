//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <functional>

#include "BoostHasher.hpp"
#include "../struct/DescriptorSetLayoutBuildType.hpp"

namespace merutilm::vkh {
    struct DescriptorSetLayoutBuildTypeHasher {
        size_t operator()(const DescriptorSetLayoutBuildType &type) const {
            size_t seed = 0;
            BoostHasher::hash(static_cast<uint64_t>(type.type), &seed);
            BoostHasher::hash(std::hash<uint32_t>{}(type.stage), &seed);
            return seed;
        }
    };
}
