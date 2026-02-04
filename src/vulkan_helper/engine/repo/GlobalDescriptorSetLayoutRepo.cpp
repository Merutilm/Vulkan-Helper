//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/engine/repo/GlobalDescriptorSetLayoutRepo.hpp>
namespace merutilm::vkh {
    inline DescriptorSetLayout & GlobalDescriptorSetLayoutRepo::pick(const DescriptorSetLayoutBuilder &layoutBuilder) {
        auto it = repository.find(layoutBuilder);
        if (it == repository.end()) {
            auto[newIt, _] = repository.try_emplace(layoutBuilder, DescriptorSetLayout(core, layoutBuilder));
            it = newIt;
        }
        return it->second;
    }
}