//
// Created by Merutilm on 2025-07-13.
//

#include "ShaderModule.hpp"

#include "../exception/exception.hpp"
#include <fstream>

namespace merutilm::vkh {
    ShaderModule::ShaderModule(const Engine &engine, const VkShaderStageFlagBits shaderStage,
                               std::string &&filename) : EngineHandler(engine), shaderStage(shaderStage),
                                                         filename(std::move(filename)) {
        ShaderModule::init();
    }

    ShaderModule::~ShaderModule() {
        ShaderModule::destroy();
    }


    void ShaderModule::init() {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw exception_invalid_args("invalid filename : " + filename);
        }
        auto code = std::vector(std::istreambuf_iterator(file), {});
        const VkShaderModuleCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .codeSize = code.size(),
            .pCode = reinterpret_cast<uint32_t *>(code.data()),
        };
        if (vkCreateShaderModule(engine.getLogicalDevice().getLogicalDeviceHandle(), &info, nullptr, &shaderModule) != VK_SUCCESS) {
            throw exception_init("Failed to create shader module!");
        }
    }

    void ShaderModule::destroy() {
        vkDestroyShaderModule(engine.getLogicalDevice().getLogicalDeviceHandle(), shaderModule, nullptr);
    }
}
