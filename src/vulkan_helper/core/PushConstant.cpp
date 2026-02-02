//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/core/PushConstant.hpp>
namespace merutilm::vkh {

    PushConstantImpl::PushConstantImpl(const VkShaderStageFlags useStage, HostDataObjectManager &&manager): hostDataObject(factory::create<HostDataObject>(std::move(manager))),
            useStage(useStage) {
    }

    PushConstantImpl::~PushConstantImpl() = default;

}