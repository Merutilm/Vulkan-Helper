//
// Created by Merutilm on 2025-07-10.
//

#include "UBO.hpp"

#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    UBO::UBO(const Engine &engine, std::unique_ptr<UploadManager> &&dataManager) : EngineHandler(engine),
        manager(std::move(dataManager)) {
        UBO::init();
    }

    UBO::~UBO() {
        UBO::destroy();
    }


    void UBO::update(const uint32_t frameIndex) const {
        memcpy(uniformBuffersMapped[frameIndex], manager->getData().data(), manager->getSize());
    }

    void UBO::init() {
        const uint32_t size = manager->getSize();

        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
        uniformBuffers.resize(maxFramesInFlight);
        uniformBuffersMemory.resize(maxFramesInFlight);
        uniformBuffersMapped.resize(maxFramesInFlight);

        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            BufferUtils::initBuffer(engine, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                    &uniformBuffers[i], &uniformBuffersMemory[i]);
            vkMapMemory(engine.getLogicalDevice().getLogicalDeviceHandle(), uniformBuffersMemory[i], 0, size, 0,
                        &uniformBuffersMapped[i]);
        }
    }

    void UBO::destroy() {
        VkDevice device = engine.getLogicalDevice().getLogicalDeviceHandle();
        const uint32_t maxFramesInFlight = engine.getSwapchain().getMaxFramesInFlight();
        for (uint32_t i = 0; i < maxFramesInFlight; ++i) {
            vkUnmapMemory(device, uniformBuffersMemory[i]);
            vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        }
    }
}
