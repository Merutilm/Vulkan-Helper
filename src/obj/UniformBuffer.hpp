//
// Created by Merutilm on 2025-07-15.
//

#pragma once
#include "BufferObject.hpp"

namespace merutilm::vkh {
    class UniformBuffer final : public BufferObject {

    public:
        explicit UniformBuffer(const Engine &engine, std::unique_ptr<BufferObjectManager> &&manager);

        ~UniformBuffer() override;

        UniformBuffer(const UniformBuffer &) = delete;

        UniformBuffer &operator=(const UniformBuffer &) = delete;

        UniformBuffer(UniformBuffer &&) = delete;

        UniformBuffer &operator=(UniformBuffer &&) = delete;

    private:
        void init() override;

        void destroy() override;
    };
}
