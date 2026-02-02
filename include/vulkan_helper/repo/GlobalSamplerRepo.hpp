//
// Created by Merutilm on 2025-08-13.
//

#pragma once
#include <vulkan_helper/base/vkh_core.hpp>
#include <vulkan_helper/repo/Repository.hpp>

#include <vulkan_helper/core/Sampler.hpp>
#include <vulkan_helper/hash/SamplerCreateInfoEquals.hpp>
#include <vulkan_helper/hash/SamplerCreateInfoHasher.hpp>

namespace merutilm::vkh {

    struct GlobalSamplerRepo final : Repository<VkSamplerCreateInfo, const VkSamplerCreateInfo &, Sampler, SamplerRef, SamplerCreateInfoHasher, SamplerCreateInfoEquals>{

        using Repository::Repository;

        SamplerRef pick(const VkSamplerCreateInfo & samplerCreateInfo) override;
    };
}
