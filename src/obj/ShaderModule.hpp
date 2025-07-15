//
// Created by Merutilm on 2025-07-13.
//

#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../handle/EngineHandler.hpp"

namespace merutilm::vkh {
    class ShaderModule final : public EngineHandler {
        VkShaderStageFlagBits shaderStage;
        VkShaderModule shaderModule = nullptr;
        const std::vector<char> code = {};
        const std::string filename;

    public:
        explicit ShaderModule(const Engine &engine, VkShaderStageFlagBits shaderStage,
                              std::string &&filename);

        ~ShaderModule() override;

        ShaderModule(const ShaderModule &) = delete;

        ShaderModule &operator=(const ShaderModule &) = delete;

        ShaderModule(ShaderModule &&) = delete;

        ShaderModule &operator=(ShaderModule &&) = delete;

        [[nodiscard]] VkShaderStageFlagBits getShaderStage() const { return shaderStage; }

        [[nodiscard]] VkShaderModule getShaderModuleHandle() const { return shaderModule; }

        [[nodiscard]] const std::vector<char> &getCode() const { return code; }

    private:
        void init() override;

        void destroy() override;
    };
}
