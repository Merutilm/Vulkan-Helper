//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include <vector>

#include "../core/TextureSampler.hpp"
#include "../core/UniformBuffer.hpp"
#include "../struct/StringHasher.hpp"

namespace merutilm::vkh {
    class DescriptorManager {
        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > names = {};
        std::vector<std::unique_ptr<UniformBuffer> > uboData = {};
        std::vector<std::unique_ptr<TextureSampler> > texSamplerData = {};
        std::unique_ptr<DescriptorSetLayoutManager> layoutManager = nullptr;

    public:
        explicit DescriptorManager();

        ~DescriptorManager() = default;

        DescriptorManager(const DescriptorManager &) = delete;

        DescriptorManager &operator=(const DescriptorManager &) = delete;

        DescriptorManager(DescriptorManager &&) noexcept = delete;

        DescriptorManager &operator=(DescriptorManager &&) noexcept = delete;

        void appendUBO(const std::string &name, VkShaderStageFlags useStage, std::unique_ptr<UniformBuffer> &&ubo);

        void appendCombinedImgSampler(const std::string &name, VkShaderStageFlags useStage, std::unique_ptr<TextureSampler> &&texture);

        const DescriptorSetLayoutManager &getLayoutManager() const {return *layoutManager;};

        [[nodiscard]] uint32_t getElementCount() const;

        [[nodiscard]] uint32_t getIndex(const std::string &varName) const;

        [[nodiscard]] UniformBuffer &getUBO(uint32_t index) const;

        [[nodiscard]] UniformBuffer &getUBO(const std::string &uboName) const;

        [[nodiscard]] TextureSampler &getSampler(uint32_t index) const;

        [[nodiscard]] TextureSampler &getSampler(const std::string &samplerName) const;
    };

    inline DescriptorManager::DescriptorManager() {
        layoutManager = std::make_unique<DescriptorSetLayoutManager>();
    }


    inline uint32_t DescriptorManager::getElementCount() const {
        return static_cast<uint32_t>(uboData.size() + texSamplerData.size());
    }

    inline void DescriptorManager::appendUBO(const std::string &name, const VkShaderStageFlags useStage,
                                             std::unique_ptr<UniformBuffer> &&ubo) {
        if (names.contains(name)) {
            throw exception_invalid_args("value already in use");
        }
        names[name] = static_cast<uint32_t>(uboData.size());
        uboData.emplace_back(std::move(ubo));
        layoutManager->append({VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, useStage});
    }

    inline void DescriptorManager::appendCombinedImgSampler(const std::string &name, const VkShaderStageFlags useStage, std::unique_ptr<TextureSampler> &&texture) {
        if (names.contains(name)) {
            throw exception_invalid_args("value already in use");
        }
        names[name] = static_cast<uint32_t>(texSamplerData.size());
        texSamplerData.emplace_back(std::move(texture));
        layoutManager->append({VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, useStage});
    }


    inline uint32_t DescriptorManager::getIndex(const std::string &varName) const {
        if (!names.contains(varName)) {
            throw exception_invalid_args("cannot found value");
        }
        return names.at(varName);
    }

    inline UniformBuffer &DescriptorManager::getUBO(const uint32_t index) const {
        if (index >= uboData.size()) {
            throw exception_invalid_args("index out of range");
        }
        return *uboData[index];
    }

    inline UniformBuffer &DescriptorManager::getUBO(const std::string &uboName) const {
        const uint32_t index = getIndex(uboName);
        return *uboData[index];
    }

    inline TextureSampler &DescriptorManager::getSampler(const uint32_t index) const {
        if (index >= texSamplerData.size()) {
            throw exception_invalid_args("index out of range");
        }
        return *texSamplerData[index];
    }

    inline TextureSampler &DescriptorManager::getSampler(const std::string &samplerName) const {
        const uint32_t index = getIndex(samplerName);
        return *texSamplerData[index];
    }
}
