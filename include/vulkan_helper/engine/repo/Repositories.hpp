//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <vector>

#include "Repository.hpp"
#include "GlobalShaderModuleRepo.hpp"

namespace merutilm::vkh {
    class Repositories {

        std::vector<RepositoryBase> repositories = {};

    public:

        explicit Repositories() {
        }

        template<typename RepoType> requires std::is_base_of_v<RepositoryBase, RepoType>
        RepoType *getRepository() {
            for (auto &repository : repositories) {
                RepoType * repo = dynamic_cast<RepoType *>(&repository);
                if (repo != nullptr) {
                    return repo;
                }
            }
            return nullptr;
        }

        template<typename RepoType> requires std::is_base_of_v<RepositoryBase, RepoType>
        void addRepository(Core & core) {
            if (getRepository<RepoType>() != nullptr) {
                throw exception_invalid_args("Repository already exists");
            }
            repositories.push_back(RepoType(core));
        }

        void clear() {
            repositories.clear();
        }

    };


};
