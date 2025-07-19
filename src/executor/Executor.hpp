//
// Created by Merutilm on 2025-07-16.
//

#pragma once
#include "../base/Core.hpp"

namespace merutilm::vkh {
    struct Executor {
        const Core &engine;

        explicit Executor(const Core &engine) : engine(engine) {
        }

        virtual ~Executor() = default;

        Executor(const Executor &) = delete;

        Executor &operator=(const Executor &) = delete;

        Executor(Executor &&) = delete;

        Executor &operator=(Executor &&) = delete;

    private:
        virtual void begin() = 0;

        virtual void end() = 0;
    };
}
