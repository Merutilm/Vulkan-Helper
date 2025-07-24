//
// Created by Merutilm on 2025-07-19.
//

#pragma once
#include "DescriptorSetLayoutRepo.hpp"
#include "Repo.hpp"
#include "../core/Descriptor.hpp"
#include "../struct/DescriptorName.hpp"
#include "../struct/EnumHasher.hpp"
#include "../template/DSTemplate.hpp"

namespace merutilm::vkh {
    struct DescriptorRepo final : Repo<DescriptorName, DescriptorName, Descriptor, EnumHasher, std::equal_to<>, DescriptorSetLayoutRepo&, VkShaderStageFlags> {


        explicit DescriptorRepo(const Core &core) : Repo(core) {

        }


        const Descriptor &pick(const DescriptorName descName, DescriptorSetLayoutRepo& layoutRepo, VkShaderStageFlags &&useStage) override {
            auto descManager = findDescriptorManager(descName, useStage);
            auto &layout = layoutRepo.pick(descManager->getLayoutManager());
            return *repository.try_emplace(
                descName, std::make_unique<Descriptor>(core, layout, std::move(descManager))).first->second;
        }

        std::unique_ptr<DescriptorManager> findDescriptorManager(const DescriptorName descName, const VkShaderStageFlags useStage) const {
            //  <factory method>

            switch (descName) {
                    using enum DescriptorName;
                case CAMERA_3D: {
                    return DSTemplate::Camera3D::create(core, useStage);
                }
                case TIME: {
                    return DSTemplate::Time::create(core, useStage);
                }
                //do something
            }
            return nullptr;
        }

    };
}
