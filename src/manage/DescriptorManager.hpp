//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include <variant>
#include <vector>

#include "../def/Sampler2D.hpp"
#include "../def/Uniform.hpp"
#include "../hash/DescriptorSetLayoutBuildTypeHasher.hpp"
#include "../hash/VectorHasher.hpp"
#include "../struct/DescriptorSetLayoutBuildType.hpp"
#include "../struct/StringHasher.hpp"
#include "../context/ImageContext.hpp"

namespace merutilm::vkh {
    using DescriptorSetLayoutBuilder = std::vector<DescriptorSetLayoutBuildType>;
    using DescriptorSetLayoutBuilderHasher = VectorHasher<DescriptorSetLayoutBuildType,
        DescriptorSetLayoutBuildTypeHasher>;

    using DescriptorType = std::variant<std::unique_ptr<Uniform>, std::unique_ptr<Sampler2D>,  ImageContext >;

    class DescriptorManager {
        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > names = {};
        std::vector<DescriptorType> data = {};
        DescriptorSetLayoutBuilder layoutBuilder = {};

    public:
        explicit DescriptorManager() = default;

        ~DescriptorManager() = default;

        DescriptorManager(const DescriptorManager &) = delete;

        DescriptorManager &operator=(const DescriptorManager &) = delete;

        DescriptorManager(DescriptorManager &&) noexcept = delete;

        DescriptorManager &operator=(DescriptorManager &&) noexcept = delete;

        void appendUBO(const std::string &name, VkShaderStageFlags useStage, std::unique_ptr<Uniform> &&ubo);

        void appendCombinedImgSampler(const std::string &name, VkShaderStageFlags useStage,
                                      std::unique_ptr<Sampler2D> &&texture);

        void appendInputAttachment(const std::string &name, VkShaderStageFlags useStage);

        const DescriptorSetLayoutBuilder &getLayoutBuilder() const { return layoutBuilder; }

        template<typename T>
        [[nodiscard]] uint32_t getElementCount() const;

        uint32_t getElements() const;

        [[nodiscard]] uint32_t getIndex(const std::string &varname) const;

        const DescriptorType &getRaw(uint32_t index);

        const DescriptorType &getRaw(const std::string &varname);

        template<typename T>
        [[nodiscard]] T &get(uint32_t index);

        template<typename T>
        [[nodiscard]] T &get(const std::string &varname);
    };

    template<typename T>
    uint32_t DescriptorManager::getElementCount() const {
        uint32_t count = 0;
        for (auto &variant: data) {
            if (std::holds_alternative<T>(variant)) ++count;
        }
        return count;
    }

    inline uint32_t DescriptorManager::getElements() const {
        return static_cast<uint32_t>(data.size());
    }

    inline void DescriptorManager::appendUBO(const std::string &name, const VkShaderStageFlags useStage,
                                             std::unique_ptr<Uniform> &&ubo) {
        if (names.contains(name)) {
            throw exception_invalid_args("value already in use");
        }
        names[name] = static_cast<uint32_t>(data.size());
        data.emplace_back(std::move(ubo));
        layoutBuilder.emplace_back(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, useStage);
    }

    inline void DescriptorManager::appendCombinedImgSampler(const std::string &name, const VkShaderStageFlags useStage,
                                                            std::unique_ptr<Sampler2D> &&texture) {
        if (names.contains(name)) {
            throw exception_invalid_args("value already in use");
        }
        names[name] = static_cast<uint32_t>(data.size());
        data.emplace_back(std::move(texture));
        layoutBuilder.emplace_back(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, useStage);
    }

    inline void DescriptorManager::appendInputAttachment(const std::string &name, const VkShaderStageFlags useStage) {
        if (names.contains(name)) {
            throw exception_invalid_args("value already in use");
        }
        names[name] = static_cast<uint32_t>(data.size());
        data.emplace_back(ImageContext{});
        layoutBuilder.emplace_back(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, useStage);
    }


    inline uint32_t DescriptorManager::getIndex(const std::string &varname) const {
        if (!names.contains(varname)) {
            throw exception_invalid_args("cannot found value");
        }
        return names.at(varname);
    }

    inline const DescriptorType &DescriptorManager::getRaw(
        const uint32_t index) {
        if (index >= data.size()) {
            throw exception_invalid_args("index out of range");
        }
        return data[index];
    }

    inline const DescriptorType &DescriptorManager::getRaw(
        const std::string &varname) {
        const uint32_t index = getIndex(varname);
        return getRaw(index);
    }

    template<typename T>
    T &DescriptorManager::get(const uint32_t index) {
        if (index >= data.size()) {
            throw exception_invalid_args("index out of range");
        }
        return std::get<T>(data[index]);
    }

    template<typename T>
    T &DescriptorManager::get(const std::string &varname) {
        const uint32_t index = getIndex(varname);
        return get<T>(index);
    }
}
