//
// Created by Merutilm on 2026-02-07.
//

#pragma once
#ifdef WIN32
#include <vulkan_helper/base/pch.hpp>
#include "vulkan_helper/engine/window/PlatformMenubarBase.hpp"

namespace merutilm::mvk {
    class Menubar final : PlatformMenubarBase {
        HMENU handle;

    public:
        explicit Menubar(HMENU handle);
        ~Menubar() override;
        Menubar(const Menubar &) = delete;
        Menubar &operator=(const Menubar &) = delete;
        Menubar(Menubar &&) = delete;
        Menubar &operator=(Menubar &&) noexcept = delete;
    private:
        void init() override;
        void cleanup() override;
    };
} // namespace merutilm::mvk
#endif
