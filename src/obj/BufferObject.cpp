//
// Created by Merutilm on 2025-07-10.
//

#include "BufferObject.hpp"

#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    BufferObject::BufferObject(const Core &core, std::unique_ptr<BufferObjectManager> &&dataManager,
                               const VkBufferUsageFlags bufferUsage) : CoreHandler(core), ConstBufferObjectManager(std::move(dataManager)),
                                                                 bufferUsage(bufferUsage) {
        BufferObject::attach();
    }

    BufferObject::~BufferObject() {
        BufferObject::detach();
    }


    void BufferObject::update(const uint32_t frameIndex) const {
        if (finalized) {
            throw exception_invalid_state("BufferObject::update() This bufferObject is already been finalized. It cannot be modified.");
        }
        memcpy(bufferMapped[frameIndex], data.data(), getSizeByte());
    }

    void BufferObject::attach() {
        const uint32_t size = getSizeByte();
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        buffers.resize(maxFramesInFlight);
        bufferMemory.resize(maxFramesInFlight);
        bufferMapped.resize(maxFramesInFlight);

        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            BufferUtils::initBuffer(core, size, bufferUsage,
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                    &buffers[i], &bufferMemory[i]);
            vkMapMemory(core.getLogicalDevice().getLogicalDeviceHandle(), bufferMemory[i], 0, size, 0,
                        &bufferMapped[i]);
            update(i);
        }
    }

    void BufferObject::finalize() {
        if (finalized) {
            throw exception_invalid_state("Double-call of BufferObject::finalize()");
        }

        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            vkUnmapMemory(device, bufferMemory[i]);
        }
        finalized = true;
    }


    void BufferObject::detach() {
        if (!finalized) {
            finalize();
        }
        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            vkFreeMemory(device, bufferMemory[i], nullptr);
            vkDestroyBuffer(device, buffers[i], nullptr);
        }
    }
}
