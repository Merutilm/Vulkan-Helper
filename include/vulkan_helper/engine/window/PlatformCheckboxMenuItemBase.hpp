//
// Created by Merutilm on 2026-05-10.
//

#pragma once

#include "Keybinding.hpp"
#include "Listener.hpp"
#include "PlatformMenuBase.hpp"

namespace merutilm::vkh {
    struct PlatformCheckboxMenuItemBase : PlatformMenuBase {
        std::vector<Keybinding> keybindings;
        Listener<bool> onClick;
        bool checked = false;

        explicit PlatformCheckboxMenuItemBase(std::wstring name, const PlatformMenuBase * parent, bool defaultFlag);
        ~PlatformCheckboxMenuItemBase() override;
        PlatformCheckboxMenuItemBase(const PlatformCheckboxMenuItemBase &) = delete;
        PlatformCheckboxMenuItemBase &operator=(const PlatformCheckboxMenuItemBase &) = delete;
        PlatformCheckboxMenuItemBase(PlatformCheckboxMenuItemBase &&other) = delete;
        PlatformCheckboxMenuItemBase &operator=(PlatformCheckboxMenuItemBase &&other) = delete;
        void addKeybinding(Keybinding &&keybinding);
        Listener<bool> &getOnClickListener();


    private:
        void init() override;
        void cleanup() override;
    };
} // namespace merutilm::vkh
