//
// Created by Merutilm on 2025-07-09.
//

#include "Descriptor.hpp"

#include "DescriptorSetLayout.hpp"
#include "../exception/exception.hpp"

namespace merutilm::vkh {
    Descriptor::Descriptor(const Core &core, const DescriptorSetLayout &descriptorSetLayout,
                           std::unique_ptr<DescriptorManager> &&descriptorManager) : CoreHandler(core),
        descriptorSetLayout(descriptorSetLayout),
        descriptorManager(std::move(descriptorManager)) {
        Descriptor::attach();
    }

    Descriptor::~Descriptor() {
        Descriptor::detach();
    }

    void Descriptor::update(const uint32_t frameIndex) const {
        const uint32_t uboCount = descriptorManager->getUBOCount();
        const uint32_t texSamplerCount = descriptorManager->getTexSamplerCount();
        const uint32_t elementCount = descriptorManager->getElementCount();
        std::vector<VkDescriptorBufferInfo> bufferInfos = {};
        std::vector<VkDescriptorImageInfo> imageInfos = {};
        std::vector<VkWriteDescriptorSet> writeDescriptorSets = {};

        for (uint32_t i = 0; i < uboCount; ++i) {
            const UniformBuffer &ubo = descriptorManager->getUBO(i);
            ubo.update(frameIndex);

            bufferInfos.push_back({
                .buffer = ubo.getBufferHandle(frameIndex),
                .offset = 0,
                .range = ubo.getTotalSizeByte()
            });

            writeDescriptorSets.push_back({
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = descriptorSets[frameIndex],
                .dstBinding = i,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .pImageInfo = nullptr,
                .pBufferInfo = &bufferInfos[i],
                .pTexelBufferView = nullptr,
            });
        }
        for (uint32_t i = 0; i < texSamplerCount; ++i) {
            const TextureSampler &tex = descriptorManager->getSampler(i);

            imageInfos.push_back({
                .sampler = tex.getSamplerHandle(),
                .imageView = tex.getImageViewHandle(),
                .imageLayout = tex.getCurrentImageLayout()
            });

            writeDescriptorSets.push_back({
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = descriptorSets[frameIndex],
                .dstBinding = uboCount + i,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .pImageInfo = &imageInfos[i],
                .pBufferInfo = nullptr,
                .pTexelBufferView = nullptr,
            });
        }
        vkUpdateDescriptorSets(core.getLogicalDevice().getLogicalDeviceHandle(), elementCount,
                               writeDescriptorSets.data(), 0, nullptr);
    }


    void Descriptor::attach() {
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        const uint32_t ubo = descriptorManager->getUBOCount();
        const uint32_t sampler = descriptorManager->getTexSamplerCount();
        const uint32_t elements = descriptorManager->getElementCount();

        std::vector<VkDescriptorPoolSize> sizes = {};

        if (ubo > 0) {

            sizes.push_back({
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = ubo
            });
        }

        if (sampler > 0) {
            sizes.push_back({
                .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = sampler
            });
        }

        const VkDescriptorPoolCreateInfo descriptorPoolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = elements,
            .poolSizeCount = static_cast<uint32_t>(sizes.size()),
            .pPoolSizes = sizes.data()
        };
        descriptorPools.resize(maxFramesInFlight);

        for (uint32_t i = 0; i < maxFramesInFlight; i++) {
            if (vkCreateDescriptorPool(core.getLogicalDevice().getLogicalDeviceHandle(), &descriptorPoolInfo, nullptr,
                                       &descriptorPools[i]) != VK_SUCCESS) {
                throw exception_init("Failed to create descriptor pool!");
            }
        }

        VkDescriptorSetLayout layout = descriptorSetLayout.getLayoutHandle();

        descriptorSets.resize(maxFramesInFlight);
        for (int i = 0; i < maxFramesInFlight; ++i) {
            if (const VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext = nullptr,
                .descriptorPool = descriptorPools[i],
                .descriptorSetCount = 1,
                .pSetLayouts = &layout
            }; vkAllocateDescriptorSets(core.getLogicalDevice().getLogicalDeviceHandle(), &descriptorSetAllocateInfo,
                                        &descriptorSets[i]) != VK_SUCCESS) {
                throw exception_init("Failed to allocate descriptor sets!");
            }
        }
    }

    void Descriptor::detach() {
        if (descriptorManager->getElementCount() == 0) {
            return;
        }
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();

        for (int i = 0; i < maxFramesInFlight; ++i) {
            vkDestroyDescriptorPool(core.getLogicalDevice().getLogicalDeviceHandle(), descriptorPools[i], nullptr);
        }
    }
}
