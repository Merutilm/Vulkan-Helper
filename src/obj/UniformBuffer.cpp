//
// Created by Merutilm on 2025-07-15.
//

#include "UniformBuffer.hpp"

namespace merutilm::vkh {
    UniformBuffer::UniformBuffer(const Engine &engine, std::unique_ptr<BufferObjectManager> &&manager) : BufferObject(engine, std::move(manager), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT){
        UniformBuffer::init();
    }

    UniformBuffer::~UniformBuffer() {
        UniformBuffer::destroy();
    }

    void UniformBuffer::init() {
        //nothing to do
    }

    void UniformBuffer::destroy() {
        //nothing to do
    }
}
