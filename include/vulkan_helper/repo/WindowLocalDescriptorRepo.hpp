//
// Created by Merutilm on 2025-07-19.
//

#pragma once
#include <vulkan_helper/repo/GlobalDescriptorSetLayoutRepo.hpp>
#include <vulkan_helper/repo/Repository.hpp>
#include <vulkan_helper/core/Descriptor.hpp>
#include <vulkan_helper/wrapped/DescriptorTemplateInfo.hpp>

namespace merutilm::vkh {
    struct WindowLocalDescriptorRepo final : Repository<uint32_t, const DescriptorTemplateInfo &, Descriptor, DescriptorRef,
                std::hash<uint32_t>, std::equal_to<>, GlobalDescriptorSetLayoutRepo &> {
        using Repository::Repository;

        DescriptorRef pick(const DescriptorTemplateInfo &descTemplateInfo,
                           GlobalDescriptorSetLayoutRepo &layoutRepo) override;
    };
}
