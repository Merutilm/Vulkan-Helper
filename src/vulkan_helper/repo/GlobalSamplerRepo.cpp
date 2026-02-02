//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/repo/GlobalSamplerRepo.hpp>

namespace merutilm::vkh {
    SamplerRef GlobalSamplerRepo::pick(const VkSamplerCreateInfo &samplerCreateInfo) {
        auto it = repository.find(samplerCreateInfo);
        if (it == repository.end()) {
            auto[newIt, _] = repository.try_emplace(samplerCreateInfo, factory::create<Sampler>(core, samplerCreateInfo));
            it = newIt;
        }
        return *it->second;
    }
}
