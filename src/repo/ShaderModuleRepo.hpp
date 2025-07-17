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
    struct ShaderModuleRepo {
        inline static std::unordered_map<std::string, std::unique_ptr<ShaderModule>, StringHasher, std::equal_to
            <> > SHADER_MODULES = {};

        static const ShaderModule &acquireShaderModule(const Engine &engine, const std::string &filename) {
            return *SHADER_MODULES.try_emplace(filename, std::make_unique<ShaderModule>(engine, filename)).first->second;
        }

        static void releaseAll() {
            SHADER_MODULES.clear();
        }
    };
}
