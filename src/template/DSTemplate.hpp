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

        static std::unique_ptr<DescriptorManager> create(DescriptorSetLayoutRepo &layoutRepo) {
            auto &descLayout = layoutRepo.pick(DescriptorSetLayoutName::UNIFORM_V1);
            auto bufferManager = std::make_unique<BufferObjectManager>();

            bufferManager->reserve<glm::mat4>(CAMERA_MODEL);
            bufferManager->reserve<glm::mat4>(CAMERA_VIEW);
            bufferManager->reserve<glm::mat4>(CAMERA_PROJ);

            auto ubo = std::make_unique<UniformBuffer>(layoutRepo.core, std::move(bufferManager));
            auto descManager = std::make_unique<DescriptorManager>(descLayout);

            descManager->appendUBO(UBO_CAMERA, std::move(ubo));
            return descManager;
        }
    };
}
