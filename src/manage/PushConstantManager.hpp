//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include "UploadManager.hpp"

namespace merutilm::vkh {
    struct PushConstantManager {
        VkShaderStageFlags useStage;
        std::unique_ptr<UploadManager> manager;


        ~PushConstantManager() = default;

        PushConstantManager(const PushConstantManager&) = delete;

        PushConstantManager& operator=(const PushConstantManager&) = delete;

        PushConstantManager(PushConstantManager&&) = delete;

        PushConstantManager& operator=(PushConstantManager&&) = delete;
    };
}
