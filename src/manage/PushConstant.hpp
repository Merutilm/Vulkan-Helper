//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include "Uploader.hpp"
#include "BufferObjectManager.hpp"

namespace merutilm::vkh {
    class PushConstant : public Uploader {
        VkShaderStageFlags useStage;

    public:
        explicit PushConstant(const VkShaderStageFlags useStage,
                              std::unique_ptr<BufferObjectManager> &&manager) : Uploader(std::move(manager)),
                                                                          useStage(useStage) {
        }

        ~PushConstant() = default;

        PushConstant(const PushConstant &) = delete;

        PushConstant &operator=(const PushConstant &) = delete;

        PushConstant(PushConstant &&) = delete;

        PushConstant &operator=(PushConstant &&) = delete;

        [[nodiscard]] VkShaderStageFlags getUseStage() const {return useStage;}

    };
}
