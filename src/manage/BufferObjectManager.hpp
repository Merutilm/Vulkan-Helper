//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include <string>
#include <unordered_map>
#include "../struct/StringHasher.hpp"
#include "../exception/exception.hpp"

namespace merutilm::vkh {
    struct BufferObjectManager final {
        std::vector<std::byte> data = {};
        std::vector<uint32_t> elements = {};
        std::vector<uint32_t> sizes = {};
        std::vector<uint32_t> offsets = {};
        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > names = {};

        explicit BufferObjectManager() = default;

        ~BufferObjectManager() = default;

        BufferObjectManager(const BufferObjectManager &) = delete;

        BufferObjectManager &operator=(const BufferObjectManager &) = delete;

        BufferObjectManager(BufferObjectManager &&) noexcept = delete;

        BufferObjectManager &operator=(BufferObjectManager &&) noexcept = delete;


        template<typename T> requires std::is_trivially_copyable_v<T>
        void reserve(const std::string &varName);

        template<typename T> requires std::is_trivially_copyable_v<T>
        void reserve(const std::string &varName, uint32_t elementCount);

        template<typename T> requires std::is_trivially_copyable_v<T>
        void add(const std::string &varName, const T &t);

        template<typename T> requires std::is_trivially_copyable_v<T>
        void add(const std::string &varName, const std::vector<T> &t);
    };


    template<typename T> requires std::is_trivially_copyable_v<T>
    void BufferObjectManager::reserve(const std::string &varName) {
        if (names.contains(varName)) {
            throw exception_invalid_args("varName already exists : " + varName);
        }

        offsets.push_back(static_cast<uint32_t>(data.size()));
        elements.push_back(1);
        names[varName] = sizes.size();
        data.resize(data.size() + sizeof(T));
        sizes.push_back(sizeof(T));
    }

    template<typename T> requires std::is_trivially_copyable_v<T>
    void BufferObjectManager::reserve(const std::string &varName, uint32_t elementCount) {
        if (names.contains(varName)) {
            throw exception_invalid_args("varName already exists : " + varName);
        }

        offsets.push_back(static_cast<uint32_t>(data.size()));
        elements.push_back(elementCount);
        names[varName] = sizes.size();
        data.resize(data.size() + sizeof(T) * elementCount);
        sizes.push_back(sizeof(T) * elementCount);
    }


    template<typename T> requires std::is_trivially_copyable_v<T>
    void BufferObjectManager::add(const std::string &varName, const T &t) {
        if (names.contains(varName)) {
            throw exception_invalid_args("varName already exists : " + varName);
        }
        const auto raw = reinterpret_cast<const std::byte *>(&t);
        offsets.push_back(static_cast<uint32_t>(data.size()));
        elements.push_back(1);
        names[varName] = sizes.size();
        data.insert(data.end(), raw, raw + sizeof(T));
        sizes.push_back(sizeof(T));
    }

    template<typename T> requires std::is_trivially_copyable_v<T>
    void BufferObjectManager::add(const std::string &varName, const std::vector<T> &t) {
        if (names.contains(varName)) {
            throw exception_invalid_args("varName already exists : " + varName);
        }
        const auto raw = reinterpret_cast<const std::byte *>(t.data());
        offsets.push_back(static_cast<uint32_t>(data.size()));
        elements.push_back(t.size());
        names[varName] = sizes.size();
        data.insert(data.end(), raw, raw + sizeof(T) * t.size());
        sizes.push_back(sizeof(T) * t.size());
    }
}
