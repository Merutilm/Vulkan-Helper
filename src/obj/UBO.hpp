//
// Created by Merutilm on 2025-07-10.
//

#pragma once
#include "../objhandler/EngineHandler.hpp"
#include "../manage/UploadManager.hpp"

namespace merutilm::vkh {
    class UBO final : public EngineHandler {
        std::vector<VkBuffer> uniformBuffers = {};
        std::vector<VkDeviceMemory> uniformBuffersMemory = {};
        std::vector<void *> uniformBuffersMapped = {};
        std::unique_ptr<UploadManager> manager;

    public:
        explicit UBO(const Engine &engine, std::unique_ptr<UploadManager> &&dataManager);

        ~UBO() override;

        UBO(const UBO &) = delete;

        UBO &operator=(const UBO &) = delete;

        UBO(UBO &&) = delete;

        UBO &operator=(UBO &&) noexcept = delete;

        [[nodiscard]] VkBuffer getBuffer(const uint32_t frameIndex) const { return uniformBuffers[frameIndex]; }

        [[nodiscard]] VkDeviceMemory getBufferMemory(const uint32_t frameIndex) const {
            return uniformBuffersMemory[frameIndex];
        }

        [[nodiscard]] const UploadManager &getManager() const { return *manager; }

        void update(uint32_t frameIndex) const;

    private:
        void init() override;

        void destroy() override;
    };
}
