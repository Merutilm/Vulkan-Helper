//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <vector>

#include "Repo.hpp"
#include "../exception/exception.hpp"

namespace merutilm::vkh {
    class Repositories {
        std::vector<std::unique_ptr<IRepo> > repositories = {};

    public:
        template<typename RepoType> requires std::is_base_of_v<IRepo, RepoType>
        RepoType *getRepository() {
            for (auto &repository : repositories) {
                auto repo = dynamic_cast<RepoType *>(repository.get());
                if (repo != nullptr) {
                    return repo;
                }
            }
            return nullptr;
        }

        template<typename RepoType> requires std::is_base_of_v<IRepo, RepoType>
        void addRepository(const Engine &engine) {
            if (getRepository<RepoType>() != nullptr) {
                throw exception_invalid_args("Repository already exists");
            }
            repositories.push_back(std::make_unique<RepoType>(engine));
        }
    };
};
