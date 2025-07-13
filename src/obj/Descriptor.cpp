//
// Created by Merutilm on 2025-07-09.
//

#include "Descriptor.hpp"

#include "../exception/exception.hpp"

namespace merutilm::vkh {

    Descriptor::Descriptor(const Engine &engine,
                           std::unique_ptr<DescriptorManager> &&descriptorManager) : EngineHandler(engine), descriptorManager(std::move(descriptorManager)) {
        Descriptor::init();
    }

    Descriptor::~Descriptor() {
        Descriptor::destroy();
    }

    void Descriptor::update(const uint32_t frameIndex) const {
        const uint32_t descCount = descriptorManager->getUBOCount();

        for (uint32_t i = 0; i < descCount; ++i) {
            descriptorManager->getUBO(i).update(frameIndex);
        }
    }


    void Descriptor::init() {
        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
        const uint32_t len = descriptorManager->getUBOCount();

        VkDescriptorPoolSize poolSize = {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = len
        };
        const VkDescriptorPoolCreateInfo descriptorPoolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = len,
            .poolSizeCount = 1,
            .pPoolSizes = &poolSize
        };
        descriptorPools.resize(maxFramesInFlight);

        for (uint32_t i = 0; i < maxFramesInFlight; i++) {
            if (vkCreateDescriptorPool(engine.getLogicalDevice().getLogicalDeviceHandle(), &descriptorPoolInfo, nullptr,
                                       &descriptorPools[i]) != VK_SUCCESS) {
                throw exception_init("Failed to create descriptor pool!");
            }
        }

        VkDescriptorSetLayout layout = descriptorManager->getLayout().getLayoutHandle();

        for (int i = 0; i < maxFramesInFlight; ++i) {
            if (const VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                .pNext = nullptr,
                .descriptorPool = descriptorPools[i],
                .descriptorSetCount = 1,
                .pSetLayouts = &layout
            }; vkAllocateDescriptorSets(engine.getLogicalDevice().getLogicalDeviceHandle(), &descriptorSetAllocateInfo,
                                        &descriptorSets[i]) != VK_SUCCESS) {
                throw exception_init("Failed to allocate descriptor sets!");
            }
        }
    }

    void Descriptor::destroy() {
        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
        for (int i = 0; i < maxFramesInFlight; ++i) {
            vkDestroyDescriptorPool(engine.getLogicalDevice().getLogicalDeviceHandle(), descriptorPools[i], nullptr);
        }
    }
}
