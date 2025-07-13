//
// Created by Merutilm on 2025-07-13.
//

#pragma once
#include <string>

namespace merutilm::vkh {
    struct EngineInitInfo {
        std::wstring windowName;
        uint32_t width;
        uint32_t height;
        float framerate;
    };
}
