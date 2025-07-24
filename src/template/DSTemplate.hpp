//
// Created by Merutilm on 2025-07-19.
//

#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "../manage/DescriptorManager.hpp"
#include "../repo/DescriptorSetLayoutRepo.hpp"

namespace merutilm::vkh::DSTemplate {

    struct Camera3D {
        inline static const std::string UBO_CAMERA = "camera";
        inline static const std::string CAMERA_MODEL = "model";
        inline static const std::string CAMERA_VIEW = "view";
        inline static const std::string CAMERA_PROJ = "proj";

        static std::unique_ptr<DescriptorManager> create(const Core &core, const VkShaderStageFlags useStage) {
            auto bufferManager = std::make_unique<ShaderObjectManager>();

            bufferManager->reserve<glm::mat4>(CAMERA_MODEL);
            bufferManager->reserve<glm::mat4>(CAMERA_VIEW);
            bufferManager->reserve<glm::mat4>(CAMERA_PROJ);

            auto ubo = std::make_unique<UniformBuffer>(core, std::move(bufferManager));
            auto descManager = std::make_unique<DescriptorManager>();
            descManager->appendUBO(UBO_CAMERA, useStage, std::move(ubo));
            return descManager;
        }
    };
    struct Time {
        inline static const std::string UBO_TIME = "time";
        inline static const std::string TIME_CURRENT = "currentTime";

        static std::unique_ptr<DescriptorManager> create(const Core &core, const VkShaderStageFlags useStage) {
            auto bufferManager = std::make_unique<ShaderObjectManager>();
            bufferManager->reserve<float>(TIME_CURRENT);
            auto ubo = std::make_unique<UniformBuffer>(core, std::move(bufferManager));
            auto descManager = std::make_unique<DescriptorManager>();
            descManager->appendUBO(UBO_TIME, useStage, std::move(ubo));
            return descManager;
        }
    };
}
