//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include "IRepo.hpp"
#include "../base/Core.hpp"

namespace merutilm::vkh {

    template<typename Key, typename KeyInput, typename Type, typename KeyHasher = std::hash<Key>, typename KeyPredicate = std::equal_to<>>
    struct Repo : IRepo{

        const Core &core;

        std::unordered_map<Key, std::unique_ptr<Type>, KeyHasher, KeyPredicate> repository = {};

        explicit Repo(const Core &core) : core(core) {}

        virtual const Type &pick(KeyInput keyInput) = 0;

    };

}
