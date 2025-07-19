//
// Created by Merutilm on 2025-07-15.
//

#pragma once


#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "BufferObjectManager.hpp"
#include "../struct/StringHasher.hpp"

namespace merutilm::vkh {
    struct ConstBufferObjectManager {
        std::vector<std::byte> data = {};
        std::vector<uint32_t> elements = {};
        std::vector<uint32_t> sizes = {};
        std::vector<uint32_t> offsets = {};
        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > names = {};

        explicit ConstBufferObjectManager(std::unique_ptr<BufferObjectManager> &&uploadManager) : data(std::move(uploadManager->data)),
                                                                            elements(std::move(uploadManager->elements)),
                                                                            sizes(std::move(uploadManager->sizes)),
                                                                            offsets(std::move(uploadManager->offsets)),
                                                                            names(std::move(uploadManager->names)) {
        }

        uint32_t getIndex(const std::string &varName) const {
            if (!names.contains(varName)) {
                throw exception_invalid_args("Not found varname : " + varName);
            }
            return names.at(varName);
        }

        template<typename T> requires std::is_trivially_copyable_v<T>
        void set(const std::string &varName, const T &t) {
            const auto binding = getIndex(varName);
            if (sizes[binding] != sizeof(T)) {
                throw exception_invalid_args(
                    std::format("size mismatch : {} and {} in {}", sizes[binding], sizeof(T), varName));
            }
            const uint32_t offset = offsets[binding];
            memcpy(&data[offset], &t, sizeof(T));
        }

        template<typename T> requires std::is_trivially_copyable_v<T>
        void set(const std::string &varName, const std::vector<T> &arr) {
            const auto binding = getIndex(varName);
            const uint32_t size = sizeof(T) * arr.size();
            if (sizes[binding] != size) {
                throw exception_invalid_args(
                    std::format("size mismatch : {} and {} in {}", sizes[binding], size, varName));
            }
            const uint32_t offset = offsets[binding];
            memcpy(&data[offset], arr.data(), size);
        }

        const std::vector<std::byte> &getData() const { return data; }

        uint32_t getOffset(const std::string &varName) const { return getOffset(getIndex(varName)); }

        uint32_t getOffset(const uint32_t index) const { return offsets[index]; }

        uint32_t getSizeByte() const { return static_cast<uint32_t>(data.size()); }

        uint32_t getObjectCount() const {
            return static_cast<uint32_t>(names.size());
        }

        uint32_t getElementCount(const uint32_t i) const {
            return elements[i];
        }

        uint32_t getElementCount(const std::string &varName) const {
            return getElementCount(getIndex(varName));
        }
    };
}
