//
// Created by Merutilm on 2025-07-10.
//

#include "BufferObject.hpp"

#include <iostream>

#include "../executor/ScopedCommandExecutor.hpp"
#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    BufferObject::BufferObject(const Core &core, std::unique_ptr<BufferObjectManager> &&dataManager,
                               const VkBufferUsageFlags bufferUsage) : CoreHandler(core),
                                                                       ConstBufferObjectManager(std::move(dataManager)),
                                                                       bufferUsage(bufferUsage) {
        BufferObject::attach();
    }

    BufferObject::~BufferObject() {
        BufferObject::detach();
    }


    void BufferObject::update(const uint32_t frameIndex) const {
        checkFinalizedBeforeUpdate();
        memcpy(bufferMapped[frameIndex], data.data(), getTotalSizeByte());
    }

    void BufferObject::update(const uint32_t frameIndex, const std::string &varname) const {
        checkFinalizedBeforeUpdate();
        const uint32_t offset = getOffset(varname);
        const uint32_t size = getSizeByte(varname);
        memcpy(static_cast<std::byte *>(bufferMapped[frameIndex]) + offset, data.data() + offset, size);
    }

    void BufferObject::checkFinalizedBeforeUpdate() const {
        if (finalized) {
            throw exception_invalid_state(
                "BufferObject::update() This bufferObject is already been finalized. It cannot be modified.");
        }
    }


    void BufferObject::attach() {
        const uint32_t size = getTotalSizeByte();
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

    void BufferObject::finalize(const CommandPool &commandPool) {
        if (finalized) {
            std::cerr << "Double-call of BufferObject::finalize()\n" << std::flush;
            return;
        }

        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        std::vector<VkBuffer> finalizedBuffer(maxFramesInFlight);
        std::vector<VkDeviceMemory> finalizedMemory(maxFramesInFlight);

        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        const VkBufferCopy copyRegion = {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = getTotalSizeByte(),
        };

        {
            const auto cex = ScopedCommandExecutor(core, commandPool);

            for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
                vkUnmapMemory(device, bufferMemory[i]);
                BufferUtils::initBuffer(core, getTotalSizeByte(), bufferUsage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                        &finalizedBuffer[i], &finalizedMemory[i]);

                vkCmdCopyBuffer(cex.getCommandBufferHandle(), buffers[i], finalizedBuffer[i], 1, &copyRegion);
            }
        }
        for (int i = 0; i < maxFramesInFlight; ++i) {
            vkFreeMemory(device, bufferMemory[i], nullptr);
            vkDestroyBuffer(device, buffers[i], nullptr);
            buffers[i] = finalizedBuffer[i];
            bufferMemory[i] = finalizedMemory[i];
        }

        finalized = true;
    }


    void BufferObject::detach() {
        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = core.getPhysicalDevice().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            if (!finalized) {
                vkUnmapMemory(device, bufferMemory[i]);
            }
            vkFreeMemory(device, bufferMemory[i], nullptr);
            vkDestroyBuffer(device, buffers[i], nullptr);
        }
    }
}
