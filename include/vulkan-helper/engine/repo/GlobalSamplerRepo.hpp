//
// Created by Merutilm on 2025-08-13.
//

#pragma once
#include <vulkan-helper/base/vkh_base.hpp>
#include <vulkan-helper/engine/img/Sampler.hpp>
#include <vulkan-helper/hash/SamplerCreateInfoEquals.hpp>
#include <vulkan-helper/hash/SamplerCreateInfoHasher.hpp>
#include "Repository.hpp"

namespace merutilm::vkh {

    struct GlobalSamplerRepo final : Repository<VkSamplerCreateInfo, const VkSamplerCreateInfo &, std::unique_ptr<Sampler>, Sampler &, SamplerCreateInfoHasher, SamplerCreateInfoEquals>{

        using Repository::Repository;

        Sampler & pick(const VkSamplerCreateInfo & samplerCreateInfo) override;
    };
}
