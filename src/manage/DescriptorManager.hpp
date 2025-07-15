//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include <vector>

#include "../obj/DescriptorSetLayout.hpp"
#include "../obj/BufferObject.hpp"
#include "../struct/StringHasher.hpp"

namespace merutilm::vkh {
    class DescriptorManager {

        const DescriptorSetLayout &layout;
        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > names = {};
        std::vector<std::unique_ptr<BufferObject>> uboData = {};

    public:
        explicit DescriptorManager(const DescriptorSetLayout &layout) : layout(layout) {};

        ~DescriptorManager() = default;

        DescriptorManager(const DescriptorManager &) = delete;

        DescriptorManager &operator=(const DescriptorManager &) = delete;

        DescriptorManager(DescriptorManager &&) noexcept = delete;

        DescriptorManager &operator=(DescriptorManager &&) noexcept = delete;

        void appendUBO(const std::string &name, std::unique_ptr<BufferObject> &&ubo);

        [[nodiscard]] uint32_t getUBOCount() const;

        [[nodiscard]] uint32_t getIndex(const std::string &uboName) const;

        [[nodiscard]] const BufferObject &getUBO(uint32_t index) const;

        [[nodiscard]] const BufferObject &getUBO(const std::string &uboName) const;

        [[nodiscard]] const DescriptorSetLayout &getLayout() const {return layout;}

    };

    inline uint32_t DescriptorManager::getUBOCount() const {
        return static_cast<uint32_t>(uboData.size());
    }

    inline void DescriptorManager::appendUBO(const std::string &name, std::unique_ptr<BufferObject> &&ubo) {
        if (names.contains(name)) {
            throw exception_invalid_args("value already in use");
        }
        names[name] = static_cast<uint32_t>(uboData.size());
        uboData.emplace_back(std::move(ubo));
    }

    inline uint32_t DescriptorManager::getIndex(const std::string &uboName) const {
        if (!names.contains(uboName)) {
            throw exception_invalid_args("cannot found value");
        }
        return names.at(uboName);
    }

    inline const BufferObject &DescriptorManager::getUBO(const uint32_t index) const {
        if (index >= uboData.size()) {
            throw exception_invalid_args("index out of range");
        }
        return *uboData[index];
    }

    inline const BufferObject &DescriptorManager::getUBO(const std::string &uboName) const {
        const uint32_t index = getIndex(uboName);
        return *uboData[index];
    }

}
