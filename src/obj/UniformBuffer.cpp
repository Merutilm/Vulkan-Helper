//
// Created by Merutilm on 2025-07-15.
//

#include "UniformBuffer.hpp"

namespace merutilm::vkh {
    UniformBuffer::UniformBuffer(const Engine &engine, std::unique_ptr<BufferObjectManager> &&manager) : BufferObject(engine, std::move(manager), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT){
        UniformBuffer::attach();
    }

    UniformBuffer::~UniformBuffer() {
        UniformBuffer::detach();
    }

    void UniformBuffer::attach() {
        //nothing to do
    }

    void UniformBuffer::detach() {
        //nothing to do
    }
}
