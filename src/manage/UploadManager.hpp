//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include <vector>
#include <format>
#include <unordered_map>
#include <string>

#include "../exception/exception.hpp"
#include "../struct/StringHasher.hpp"

namespace merutilm::vkh {
    class UploadManager {
        uint32_t elements = 0;
        std::vector<char> data = {};
        std::vector<uint32_t> sizes = {};
        std::vector<uint32_t> offsets = {};
        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > names = {};

    public:
        explicit UploadManager() = default;

        ~UploadManager() = default;

        UploadManager(const UploadManager &) = delete;

        UploadManager &operator=(const UploadManager &) = delete;

        UploadManager(UploadManager &&) noexcept = delete;

        UploadManager &operator=(UploadManager &&) noexcept = delete;

        template<typename T> requires std::is_trivially_copyable_v<T>
        void set(const std::string &varName, const T &t) const;

        uint32_t getIndex(const std::string &varName) const;

        uint32_t getElements() const;

        void reserve(const std::string &varName, uint32_t size);

        template<typename T> requires std::is_trivially_copyable_v<T>
        void add(const std::string &varName, const T &t);

        const std::vector<char> &getData() const { return data; }

        uint32_t getOffset(const std::string &varName) const { return getOffset(getIndex(varName)); }

        uint32_t getOffset(const uint32_t index) const { return offsets[index]; }

        uint32_t getSize() const { return static_cast<uint32_t>(data.size()); }
    };

    template<typename T> requires std::is_trivially_copyable_v<T>
    void UploadManager::set(const std::string &varName, const T &t) const {
        const auto binding = getIndex(varName);
        if (sizes[binding] != sizeof(T)) {
            throw exception_invalid_args(
                std::format("size mismatch : {} and {} in {}", sizes[binding], sizeof(T), varName));
        }
        const uint32_t offset = offsets[binding];
        memcpy(&data[offset], &t, sizeof(T));
    }

    inline uint32_t UploadManager::getIndex(const std::string &varName) const {
        return names.at(varName);
    }

    inline uint32_t UploadManager::getElements() const {
        return static_cast<uint32_t>(names.size());
    }


    inline void UploadManager::reserve(const std::string &varName, const uint32_t size) {
        if (names.contains(varName)) {
            throw exception_invalid_args("varName already exists : " + varName);
        }

        offsets.push_back(static_cast<uint32_t>(data.size()));
        names[varName] = elements;
        data.resize(data.size() + size);
        sizes.push_back(size);
        ++elements;
    }

    template<typename T> requires std::is_trivially_copyable_v<T>
    void UploadManager::add(const std::string &varName, const T &t) {
        if (names.contains(varName)) {
            throw exception_invalid_args("varName already exists : " + varName);
        }
        const auto raw = static_cast<std::byte*>(&t);
        offsets.push_back(static_cast<uint32_t>(data.size()));
        names[varName] = elements;
        data.insert(data.end(), raw, raw + sizeof(T));
        sizes.push_back(sizeof(T));
        ++elements;
    }
}
