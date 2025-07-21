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
    struct DescriptorRepo final : Repo<DescriptorName, DescriptorName, Descriptor, EnumHasher, std::equal_to<>, const DescriptorSetLayout &> {

        explicit DescriptorRepo(const Core &core) : Repo(core) {

        }


        const Descriptor &pick(const DescriptorName descName, const DescriptorSetLayout &layout) override {
            return *repository.try_emplace(
                descName, std::make_unique<Descriptor>(core, findDescriptorManager(descName, layout))).first->second;
        }

        static std::unique_ptr<DescriptorManager> findDescriptorManager(const DescriptorName descName, const DescriptorSetLayout &layout) {
            //  <factory method>

            switch (descName) {
                    using enum DescriptorName;
                case CAMERA_3D: {
                    return DSTemplate::Camera3D::create(layout);
                }
                case TIME: {
                    return DSTemplate::Time::create(layout);
                }
                //do something
            }
            return nullptr;
        }
    };
}
