//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/repo/GlobalDescriptorSetLayoutRepo.hpp>
namespace merutilm::vkh {
    inline DescriptorSetLayoutRef GlobalDescriptorSetLayoutRepo::pick(const DescriptorSetLayoutBuilder &layoutBuilder) {
        auto it = repository.find(layoutBuilder);
        if (it == repository.end()) {
            auto[newIt, _] = repository.try_emplace(layoutBuilder, factory::create<DescriptorSetLayout>(core, layoutBuilder));
            it = newIt;
        }
        return *it->second;
    }
}