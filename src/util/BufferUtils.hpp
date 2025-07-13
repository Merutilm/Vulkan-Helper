//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include "../exception/exception.hpp"
#include "../obj/Engine.hpp"

namespace merutilm::vkh {
    struct BufferUtils {
        BufferUtils() = delete;

        static void initBuffer(const Engine &engine, const VkDeviceSize size, const VkBufferUsageFlags usage,
                               const VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *bufferMemory) {
            createBuffer(engine, size, usage, buffer);
            allocateBufferMemory(engine, properties, *buffer, bufferMemory);
            vkBindBufferMemory(engine.getLogicalDevice().getLogicalDeviceHandle(), *buffer, *bufferMemory, 0);
        }

        static void createBuffer(const Engine &engine, const VkDeviceSize size, const VkBufferUsageFlags usage,
                                 VkBuffer *buffer) {
            if (const VkBufferCreateInfo bufferInfo = {
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .size = size,
                .usage = usage,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount = 0,
                .pQueueFamilyIndices = nullptr
            }; vkCreateBuffer(engine.getLogicalDevice().getLogicalDeviceHandle(), &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
                throw exception_init("Failed to create buffer!");
            }
        }


        static void allocateBufferMemory(const Engine &engine,  const VkMemoryPropertyFlags properties,
                                         const VkBuffer buffer, VkDeviceMemory *bufferMemory) {
            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(engine.getLogicalDevice().getLogicalDeviceHandle(), buffer, &memRequirements);

            if (const VkMemoryAllocateInfo allocInfo = {
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .pNext = nullptr,
                .allocationSize = memRequirements.size,
                .memoryTypeIndex = findMemoryTypeIndex(engine, memRequirements.memoryTypeBits,
                                                       properties)
            }; vkAllocateMemory(engine.getLogicalDevice().getLogicalDeviceHandle(), &allocInfo, nullptr, bufferMemory) != VK_SUCCESS) {
                throw exception_init("failed to allocate memory!");
            }
        }

        
        static uint32_t findMemoryTypeIndex(const Engine &engine, const uint32_t memoryTypeBits,
                                            const VkMemoryPropertyFlags properties) {
            const VkPhysicalDeviceMemoryProperties memProperties = engine.getPhysicalDevice().getPhysicalDeviceMemoryProperties();
            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                if ((memoryTypeBits & 1 << i) != 0 && //check memory type is equal
                    (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    //check the "propertyFlags" is completely contains "properties"
                    return i;
                    }
            }

            throw exception_init("failed to find suitable memory type!");
        }

    };
}
