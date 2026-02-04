//
// Created by Merutilm on 2026-02-03.
//

#pragma once
#include "vulkan_helper/base/vkh.hpp"

namespace merutilm {
    struct DemoApplication {

        vkh::Engine engine;

        DemoApplication();

        void init();
        void createWindow();

        void start();
    };
}
