//
// Created by Merutilm on 2025-07-09.
//

#include "Descriptor.hpp"

#include "DescriptorSetLayout.hpp"
#include "../exception/exception.hpp"

namespace merutilm::vkh {
    Descriptor::Descriptor(const Core &core, const DescriptorSetLayout &descriptorSetLayout,
                           std::unique_ptr<DescriptorManager> &&descriptorManager) : CoreHandler(core), descriptorSetLayout(descriptorSetLayout),
        descriptorManager(std::move(descriptorManager)) {
        Descriptor::attach();
    }

    Descriptor::~Descriptor() {
        Descriptor::detach();
    }

    void Descriptor::update(const uint32_t frameIndex) const {
        const uint32_t descCount = descriptorManager->getElementCount();
        std::vector<VkDescriptorBufferInfo> bufferInfos(descCount);
        std::vector<VkWriteDescriptorSet> writeDescriptorSets(descCount);

        for (uint32_t i = 0; i < descCount; ++i) {
            const UniformBuffer &ubo = descriptorManager->getUBO(i);
            ubo.update(frameIndex);

            bufferInfos[i] = {
                .buffer = ubo.getBufferHandle(frameIndex),
                .offset = 0,
                .range = ubo.getTotalSizeByte()
            };

            writeDescriptorSets[i] = {
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
            };
        }
        vkUpdateDescriptorSets(core.getLogicalDevice().getLogicalDeviceHandle(), descCount,
                               writeDescriptorSets.data(), 0, nullptr);
    }


    void Descriptor::attach() {
        const uint32_t layoutLen = descriptorManager->getLayoutManager().getDescriptorCount();
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        const uint32_t descLen = descriptorManager->getElementCount();
        if (descLen == 0) {
            return;
        }

        if (descLen != layoutLen) {
            throw exception_invalid_args(std::format("layout count {} and descriptor count {} are not matched",
                                                     layoutLen, descLen));
        }

        VkDescriptorPoolSize poolSize = {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = descLen
        };
        const VkDescriptorPoolCreateInfo descriptorPoolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = descLen,
            .poolSizeCount = 1,
            .pPoolSizes = &poolSize
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
