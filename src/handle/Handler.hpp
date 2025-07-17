//
// Created by Merutilm on 2025-07-07.
//
#pragma once

namespace merutilm::vkh {
    struct Handler {
        virtual ~Handler() = default;

        virtual void attach() = 0;

        virtual void detach() = 0;
    };
}
