//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include "../handle/CoreHandler.hpp"
#include "../manage/ConstBufferObjectManager.hpp"
#include "../manage/BufferObjectManager.hpp"

namespace merutilm::vkh {
    class BufferObject : public CoreHandler, public ConstBufferObjectManager{

        VkBufferUsageFlags bufferUsage;
        std::vector<VkBuffer> buffers = {};
        std::vector<VkDeviceMemory> bufferMemory = {};
        std::vector<void *> bufferMapped = {};
        bool finalized = false;

    public:
        explicit BufferObject(const Core &core, std::unique_ptr<BufferObjectManager> &&dataManager, VkBufferUsageFlags bufferUsage);

        ~BufferObject() override;

        BufferObject(const BufferObject &) = delete;

        BufferObject &operator=(const BufferObject &) = delete;

        BufferObject(BufferObject &&) = delete;

        BufferObject &operator=(BufferObject &&) noexcept = delete;

        void finalize();

        [[nodiscard]] VkBuffer getBufferHandle(const uint32_t frameIndex) const { return buffers[frameIndex]; }


        void update(uint32_t frameIndex) const;

    protected:
        void attach() override;

        void detach() override;
    };
}
