//
// Created by Merutilm on 2025-07-19.
//

#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

#include "../manage/DescriptorSetLayoutManager.hpp"

namespace merutilm::vkh {
    struct DSLTemplate {
        static std::unique_ptr<DescriptorSetLayoutManager> uniform_v1() {
            auto manager = std::make_unique<DescriptorSetLayoutManager>();
            manager->append({
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .stage = VK_SHADER_STAGE_VERTEX_BIT,
            });
            return manager;
        }
        static std::unique_ptr<DescriptorSetLayoutManager> uniform_vf1() {
            auto manager = std::make_unique<DescriptorSetLayoutManager>();
            manager->append({
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .stage = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            });
            return manager;
        }
        static std::unique_ptr<DescriptorSetLayoutManager> uniform_f1() {
            auto manager = std::make_unique<DescriptorSetLayoutManager>();
            manager->append({
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            });
            return manager;
        }
    };
}
