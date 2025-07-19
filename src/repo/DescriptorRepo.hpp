//
// Created by Merutilm on 2025-07-19.
//

#pragma once
#include "DescriptorSetLayoutRepo.hpp"
#include "Repo.hpp"
#include "../obj/Descriptor.hpp"
#include "../struct/DescriptorName.hpp"
#include "../struct/EnumHasher.hpp"
#include "../template/DSTemplate.hpp"

namespace merutilm::vkh {
    struct DescriptorRepo final : Repo<DescriptorName, DescriptorName, Descriptor, EnumHasher> {

        DescriptorSetLayoutRepo &layoutRepo;
        explicit DescriptorRepo(const Core &core, DescriptorSetLayoutRepo &repo) : Repo(core), layoutRepo(repo) {

        }

        const Descriptor &pick(const DescriptorName descName) override {
            return *repository.try_emplace(
                descName, std::make_unique<Descriptor>(core, findDescriptorManager(descName))).first->second;
        }

        std::unique_ptr<DescriptorManager> findDescriptorManager(const DescriptorName descName) const {
            //  <factory method>

            switch (descName) {
                    using enum DescriptorName;
                case CAMERA_3D: {
                    return DSTemplate::Camera3D::create(layoutRepo);
                }
                //do something
            }
            return nullptr;
        }
    };
}
