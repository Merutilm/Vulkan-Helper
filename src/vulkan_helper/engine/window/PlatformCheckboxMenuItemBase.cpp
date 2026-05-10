//
// Created by Merutilm on 2026-05-11.
//

#include <vulkan_helper/engine/window/PlatformCheckboxMenuItemBase.hpp>

#include "vulkan_helper/engine/window/PlatformMenuItemBase.hpp"

namespace merutilm::vkh {

    PlatformCheckboxMenuItemBase::PlatformCheckboxMenuItemBase(std::wstring name, const PlatformMenuBase *parent, const bool defaultFlag) : PlatformMenuBase(std::move(name), parent), checked(defaultFlag) {
        PlatformCheckboxMenuItemBase::init();
    }
    PlatformCheckboxMenuItemBase::~PlatformCheckboxMenuItemBase() {
        PlatformCheckboxMenuItemBase::cleanup();
    }
    void PlatformCheckboxMenuItemBase::addKeybinding(Keybinding &&keybinding) {
        keybindings.emplace_back(std::move(keybinding));
    }
    Listener<bool> &PlatformCheckboxMenuItemBase::getOnClickListener() {
        return onClick;
    }
    void PlatformCheckboxMenuItemBase::init() {}
    void PlatformCheckboxMenuItemBase::cleanup() {}
} // namespace merutilm::vkh
