//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include "IRepo.hpp"
#include "../context/Engine.hpp"

namespace merutilm::vkh {

    template<typename Key, typename Type, typename KeyHasher = std::hash<Key>, typename KeyPredicate = std::equal_to<>, typename... Args>
    struct Repo : IRepo{

        const Engine &engine;

        std::unordered_map<Key, std::unique_ptr<Type>, KeyHasher, KeyPredicate> repository = {};

        explicit Repo(const Engine &engine) : engine(engine) {}

        virtual const Type &pick(Args&&... args) = 0;

    };

}
