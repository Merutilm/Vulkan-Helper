//
// Created by Merutilm on 2025-07-18.
//

#include "IndexBuffer.hpp"

namespace merutilm::vkh {
    IndexBuffer::IndexBuffer(const Core &core, std::unique_ptr<BufferObjectManager> &&manager) : BufferObject(core, std::move(manager), VK_BUFFER_USAGE_INDEX_BUFFER_BIT) {
        IndexBuffer::attach();
    }

    IndexBuffer::~IndexBuffer() {
        IndexBuffer::detach();
    }

    void IndexBuffer::attach() {
        //no operation
    }

    void IndexBuffer::detach() {
        //no operation
    }



}