//
// Created by Merutilm on 2025-07-16.
//

#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "../obj/ShaderModule.hpp"
#include "../struct/StringHasher.hpp"

namespace merutilm::vkh {
    struct ShaderModuleRepo final : Repo<std::string, ShaderModule, StringHasher, std::equal_to<>, const std::string&>{

        using Repo::Repo;


        const ShaderModule &pick(const std::string &filename) override {
            return *repository.try_emplace(filename, std::make_unique<ShaderModule>(engine, filename)).first->second;
        }

    };
}
