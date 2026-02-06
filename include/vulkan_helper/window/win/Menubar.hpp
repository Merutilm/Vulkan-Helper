//
// Created by Merutilm on 2026-02-07.
//

#pragma once
#ifdef WIN32
#include <vulkan_helper/base/pch.hpp>
#include "vulkan_helper/window/PlatformMenubarBase.hpp"

namespace merutilm::vkh {
    class Menubar final : PlatformMenubarBase {
        HMENU handle;

    public:
        explicit Menubar(HMENU handle);
        ~Menubar() override;
        Menubar(const Menubar &) = delete;
        Menubar &operator=(const Menubar &) = delete;
        Menubar(Menubar &&) noexcept = default;
        Menubar &operator=(Menubar &&) noexcept = delete;
    private:
        void init() override;
        void cleanup() override;
    };
} // namespace merutilm::vkh
#endif
