//
// Created by Merutilm on 2025-07-14.
//

#pragma once
#include "Fence.hpp"
#include "Semaphore.hpp"
#include <vulkan_helper/handle/CoreHandler.hpp>

namespace merutilm::vkh {
    class SyncObject final : public CoreHandler {
        std::vector<Fence> fences = {};
        std::vector<Semaphore> semaphores = {};


    public:
        explicit SyncObject(Core & core);

        ~SyncObject() override;

        SyncObject(const SyncObject &) = delete;

        SyncObject &operator=(const SyncObject &) = delete;

        SyncObject(SyncObject &&) noexcept = default;

        SyncObject &operator=(SyncObject &&) = delete;

        [[nodiscard]] Semaphore & getSemaphore(const uint32_t frameIndex) {
            return semaphores[frameIndex];
        }

        [[nodiscard]] Fence & getFence(const uint32_t frameIndex) { return fences[frameIndex]; }

    private:
        void init() override;

        void cleanup() override;
    };


}
