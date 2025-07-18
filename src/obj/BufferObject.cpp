//
// Created by Merutilm on 2025-07-10.
//

#include "BufferObject.hpp"

#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    BufferObject::BufferObject(const Engine &engine, std::unique_ptr<BufferObjectManager> &&dataManager,
                               const VkBufferUsageFlags bufferUsage) : EngineHandler(engine), Uploader(std::move(dataManager)),
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
        memcpy(bufferMapped[frameIndex], data.data(), getSize());
    }

    void BufferObject::attach() {
        const uint32_t size = getSize();
        const uint32_t maxFramesInFlight = engine.getPhysicalDevice().getMaxFramesInFlight();
        buffers.resize(maxFramesInFlight);
        bufferMemory.resize(maxFramesInFlight);
        bufferMapped.resize(maxFramesInFlight);

        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            BufferUtils::initBuffer(engine, size, bufferUsage,
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                    &buffers[i], &bufferMemory[i]);
            vkMapMemory(engine.getLogicalDevice().getLogicalDeviceHandle(), bufferMemory[i], 0, size, 0,
                        &bufferMapped[i]);
            update(i);
        }
    }

    void BufferObject::finalize() {
        if (finalized) {
            throw exception_invalid_state("BufferObject::finalize() called more than once");
        }

        const VkDevice device = engine.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = engine.getPhysicalDevice().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            vkUnmapMemory(device, bufferMemory[i]);
        }
        finalized = true;
    }


    void BufferObject::detach() {
        if (!finalized) {
            finalize();
        }
        const VkDevice device = engine.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = engine.getPhysicalDevice().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            vkFreeMemory(device, bufferMemory[i], nullptr);
            vkDestroyBuffer(device, buffers[i], nullptr);
        }
    }
}
