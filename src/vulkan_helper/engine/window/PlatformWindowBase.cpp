#include <vulkan_helper/engine/window/PlatformWindowBase.hpp>
#include <vulkan_helper/base/logger.hpp>
//
// Created by Merutilm on 2026-02-07.
//
namespace merutilm::vkh {

    PlatformWindowBase::PlatformWindowBase(WindowInitializerSettings &&settings) : initializerSettings(std::move(settings)) { PlatformWindowBase::init(); }
    PlatformWindowBase::~PlatformWindowBase() { PlatformWindowBase::cleanup(); }

    void PlatformWindowBase::init() {

    }

    void PlatformWindowBase::start() { eventSystem.applicationLifecycle.onStart.invoke(); }

    KeyInput::Key PlatformWindowBase::findEquivalentKey(uint32_t nativeKey) {

        const auto result = keyMapper.find(nativeKey);
        if (result == keyMapper.end()) {
            logger::log("UNKNOWN key detected!! keyCode : {0}", nativeKey);
            return KeyInput::Key::KEY_UNKNOWN;
        }
        return result->second;
    }

    bool PlatformWindowBase::isKeyPressed(const KeyInput::Key key) const {
        return pressedKeys.contains(key);
    }

    void PlatformWindowBase::cleanup() {
    }

} // namespace merutilm::vkh
