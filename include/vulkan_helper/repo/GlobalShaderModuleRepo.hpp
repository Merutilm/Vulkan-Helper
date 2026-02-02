//
// Created by Merutilm on 2025-07-16.
//

#pragma once
#include <vulkan_helper/base/vkh_base.hpp>
#include <vulkan_helper/repo/Repository.hpp>

#include <vulkan_helper/core/ShaderModule.hpp>
#include <vulkan_helper/hash/StringHasher.hpp>

namespace merutilm::vkh {
    struct GlobalShaderModuleRepo final : Repository<std::string, const std::string &, ShaderModule, ShaderModuleRef,
                StringHasher, std::equal_to<> > {
        using Repository::Repository;


        ShaderModuleRef pick(const std::string &filename) override;
    };
}
