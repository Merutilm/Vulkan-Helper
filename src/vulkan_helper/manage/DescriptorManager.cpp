//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/manage/DescriptorManager.hpp>

namespace merutilm::vkh {
    DescriptorManagerImpl::DescriptorManagerImpl() = default;

    DescriptorManagerImpl::~DescriptorManagerImpl() = default;

    void DescriptorManagerImpl::appendUBO(const uint32_t bindingExpected, const VkShaderStageFlags useStage,
        Uniform &&ubo) {
        safe_array::check_index_equal(bindingExpected, static_cast<uint32_t>(data.size()),
                                      "Descriptor UBO add");
        data.emplace_back(std::move(ubo));
        layoutBuilder.emplace_back(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, useStage);
    }

    void DescriptorManagerImpl::appendSSBO(const uint32_t bindingExpected, const VkShaderStageFlags useStage,
        ShaderStorage &&ssbo) {
        safe_array::check_index_equal(bindingExpected, static_cast<uint32_t>(data.size()),
                                      "Descriptor SSBO add");
        data.emplace_back(std::move(ssbo));
        layoutBuilder.emplace_back(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, useStage);
    }

    void DescriptorManagerImpl::appendCombinedImgSampler(const uint32_t bindingExpected,
        const VkShaderStageFlags useStage, CombinedImageSampler &&sampler) {
        safe_array::check_index_equal(bindingExpected, static_cast<uint32_t>(data.size()),
                                      "Descriptor Sampler add");
        data.emplace_back(std::move(sampler));
        layoutBuilder.emplace_back(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, useStage);
    }

    void DescriptorManagerImpl::appendInputAttachment(const uint32_t bindingExpected,
        const VkShaderStageFlags useStage) {
        safe_array::check_index_equal(bindingExpected, static_cast<uint32_t>(data.size()),
                                      "Descriptor Input Attachment add");
        data.emplace_back(InputAttachment{});
        layoutBuilder.emplace_back(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, useStage);
    }

    void DescriptorManagerImpl::appendStorageImage(const uint32_t bindingExpected,
        const VkShaderStageFlags useStage) {
        safe_array::check_index_equal(bindingExpected, static_cast<uint32_t>(data.size()),
                                      "Descriptor Image2D add");
        data.emplace_back(StorageImage{});
        layoutBuilder.emplace_back(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, useStage);
    }
}