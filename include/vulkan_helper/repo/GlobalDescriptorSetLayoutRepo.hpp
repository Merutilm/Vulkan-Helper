//
// Created by Merutilm on 2025-07-16.
//

#pragma once
#include <vulkan_helper/base/vkh_core.hpp>
#include <vulkan_helper/core/DescriptorSetLayout.hpp>
#include <vulkan_helper/repo/Repository.hpp>

namespace merutilm::vkh {
    struct GlobalDescriptorSetLayoutRepo final : Repository<DescriptorSetLayoutBuilder, const DescriptorSetLayoutBuilder &,
                DescriptorSetLayout, DescriptorSetLayoutRef,
                DescriptorSetLayoutBuilderHasher, std::equal_to<> > {
        using Repository::Repository;

        DescriptorSetLayoutRef pick(const DescriptorSetLayoutBuilder &layoutBuilder) override;
    };


}
