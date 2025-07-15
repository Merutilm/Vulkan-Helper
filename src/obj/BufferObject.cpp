//
// Created by Merutilm on 2025-07-10.
//

#include "BufferObject.hpp"

#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    BufferObject::BufferObject(const Engine &engine, std::unique_ptr<BufferObjectManager> &&dataManager,
                               const VkBufferUsageFlags bufferUsage) : EngineHandler(engine), Uploader(std::move(dataManager)),
                                                                 bufferUsage(bufferUsage) {
        BufferObject::init();
    }

    BufferObject::~BufferObject() {
        BufferObject::destroy();
    }


    void BufferObject::update(const uint32_t frameIndex) const {
        memcpy(bufferMapped[frameIndex], data.data(), getSize());
    }

    void BufferObject::init() {
        const uint32_t size = getSize();

        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
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

    void BufferObject::destroy() {
        const VkDevice device = engine.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            vkUnmapMemory(device, bufferMemory[i]);
            vkFreeMemory(device, bufferMemory[i], nullptr);
            vkDestroyBuffer(device, buffers[i], nullptr);
        }
    }
}
