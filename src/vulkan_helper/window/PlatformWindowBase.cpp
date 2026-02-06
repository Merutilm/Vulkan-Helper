#include "vulkan_helper/window/PlatformWindowBase.hpp"
//
// Created by Merutilm on 2026-02-07.
//
namespace merutilm::vkh {

    PlatformWindowBase::PlatformWindowBase(WindowInitializerSettings &&settings) : initializerSettings(std::move(settings)) { PlatformWindowBase::init(); }
    PlatformWindowBase::~PlatformWindowBase() { PlatformWindowBase::cleanup(); }

    void PlatformWindowBase::init() {

    }

    void PlatformWindowBase::start() {
        windowEvent.applicationLifecycleEvents.onStart.invoke();
    }

    void PlatformWindowBase::cleanup() {
        windowEvent.windowEvents.onClose.invoke();
    }

} // namespace merutilm::vkh
