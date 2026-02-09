//
// Created by Merutilm on 2026-02-07.
//
#include <vulkan_helper/engine/window/PlatformMenubarBase.hpp>

namespace merutilm::vkh {
    PlatformMenubarBase::PlatformMenubarBase() {
        PlatformMenubarBase::init();
    }
    PlatformMenubarBase::~PlatformMenubarBase() {
        PlatformMenubarBase::cleanup();
    }
    void PlatformMenubarBase::init() {

    }
    void PlatformMenubarBase::cleanup() {

    }
} // namespace merutilm::vkh
