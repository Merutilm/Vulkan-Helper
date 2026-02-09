//
// Created by Merutilm on 2026-02-07.
//

#pragma once
#include "vulkan_helper/handle/Handler.hpp"
namespace merutilm::vkh {
    class PlatformMenubarBase : Handler {

    public:
        PlatformMenubarBase();
        ~PlatformMenubarBase() override;
        PlatformMenubarBase(const PlatformMenubarBase &) = delete;
        PlatformMenubarBase &operator=(const PlatformMenubarBase &) = delete;
        PlatformMenubarBase(PlatformMenubarBase&&) = delete;
        PlatformMenubarBase & operator=(PlatformMenubarBase&&) = delete;
    private:
        void init() override;
        void cleanup() override;
    };
}