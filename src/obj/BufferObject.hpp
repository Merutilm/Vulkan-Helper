//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include "../handle/EngineHandler.hpp"
#include "../manage/Uploader.hpp"
#include "../manage/BufferObjectManager.hpp"

namespace merutilm::vkh {
    class BufferObject : public EngineHandler, public Uploader{

        VkBufferUsageFlags bufferUsage;
        std::vector<VkBuffer> buffers = {};
        std::vector<VkDeviceMemory> bufferMemory = {};
        std::vector<void *> bufferMapped = {};
        bool finalized = false;

    public:
        explicit BufferObject(const Engine &engine, std::unique_ptr<BufferObjectManager> &&dataManager, VkBufferUsageFlags bufferUsage);

        ~BufferObject() override;

        BufferObject(const BufferObject &) = delete;

        BufferObject &operator=(const BufferObject &) = delete;

        BufferObject(BufferObject &&) = delete;

        BufferObject &operator=(BufferObject &&) noexcept = delete;

        void finalize();

        [[nodiscard]] VkBuffer getBuffer(const uint32_t frameIndex) const { return buffers[frameIndex]; }


        void update(uint32_t frameIndex) const;

    protected:
        void attach() override;

        void detach() override;
    };
}
