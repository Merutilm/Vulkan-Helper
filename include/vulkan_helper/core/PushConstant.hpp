//
// Created by Merutilm on 2025-07-13.
//

#pragma once

#include <vulkan_helper/base/vkh_core.hpp>
#include <vulkan_helper/core/HostDataObject.hpp>
#include <vulkan_helper/manage/HostDataObjectManager.hpp>

namespace merutilm::vkh {
    class PushConstantImpl final {
        HostDataObject hostDataObject = nullptr;
        VkShaderStageFlags useStage;

    public:
        explicit PushConstantImpl(VkShaderStageFlags useStage,HostDataObjectManager &&manager);

        ~PushConstantImpl();

        PushConstantImpl(const PushConstantImpl &) = delete;

        PushConstantImpl &operator=(const PushConstantImpl &) = delete;

        PushConstantImpl(PushConstantImpl &&) = delete;

        PushConstantImpl &operator=(PushConstantImpl &&) = delete;

        [[nodiscard]] VkShaderStageFlags getUseStage() const {
            return useStage;
        }

        [[nodiscard]] HostDataObjectRef getHostObject() const {
            return *hostDataObject;
        }

    };

    using PushConstant = std::unique_ptr<PushConstantImpl>;
    using PushConstantPtr = PushConstantImpl *;
    using PushConstantRef = PushConstantImpl &;
}
