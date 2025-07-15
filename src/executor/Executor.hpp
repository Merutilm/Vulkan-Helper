//
// Created by Merutilm on 2025-07-16.
//

#pragma once
#include "../obj/Engine.hpp"

namespace merutilm::vkh {
    struct Executor {
        const Engine &engine;

        explicit Executor(const Engine &engine) : engine(engine) {
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
