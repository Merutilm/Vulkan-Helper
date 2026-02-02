//
// Created by Merutilm on 2025-07-10.
//

#pragma once

#include <vulkan_helper/base/vkh_base.hpp>
#include <vulkan_helper/core/CombinedImageSampler.hpp>
#include <vulkan_helper/core/Uniform.hpp>
#include <vulkan_helper/hash/DescriptorSetLayoutBuildTypeHasher.hpp>
#include <vulkan_helper/hash/VectorHasher.hpp>
#include <vulkan_helper/wrapped/DescriptorSetLayoutBuildType.hpp>
#include <vulkan_helper/core/ShaderStorage.hpp>
#include <vulkan_helper/wrapped/InputAttachment.hpp>
#include <vulkan_helper/wrapped/StorageImage.hpp>

namespace merutilm::vkh {
    using DescriptorSetLayoutBuilder = std::vector<DescriptorSetLayoutBuildType>;
    using DescriptorSetLayoutBuilderHasher = VectorHasher<DescriptorSetLayoutBuildType,
        DescriptorSetLayoutBuildTypeHasher>;

    using DescriptorType = std::variant<Uniform, ShaderStorage, CombinedImageSampler, InputAttachment, StorageImage>;


    struct DescriptorManagerImpl {
        std::vector<DescriptorType> data = {};
        DescriptorSetLayoutBuilder layoutBuilder = {};

        explicit DescriptorManagerImpl();

        ~DescriptorManagerImpl();

        DescriptorManagerImpl(const DescriptorManagerImpl &) = delete;

        DescriptorManagerImpl &operator=(const DescriptorManagerImpl &) = delete;

        DescriptorManagerImpl(DescriptorManagerImpl &&) noexcept = delete;

        DescriptorManagerImpl &operator=(DescriptorManagerImpl &&) noexcept = delete;


        void appendUBO(uint32_t bindingExpected, VkShaderStageFlags useStage,
                       Uniform &&ubo);

        void appendSSBO(uint32_t bindingExpected, VkShaderStageFlags useStage,
                        ShaderStorage &&ssbo);

        void appendCombinedImgSampler(uint32_t bindingExpected, VkShaderStageFlags useStage,
                                      CombinedImageSampler &&sampler);

        void appendInputAttachment(uint32_t bindingExpected, VkShaderStageFlags useStage);

        void appendStorageImage(uint32_t bindingExpected, VkShaderStageFlags useStage);
    };



    using DescriptorManager = std::unique_ptr<DescriptorManagerImpl>;
    using DescriptorManagerPtr = DescriptorManagerImpl *;
    using DescriptorManagerRef = DescriptorManagerImpl &;
}
