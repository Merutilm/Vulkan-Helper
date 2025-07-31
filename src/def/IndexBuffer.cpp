//
// Created by Merutilm on 2025-07-18.
//

#include "IndexBuffer.hpp"

namespace merutilm::vkh {
    IndexBuffer::IndexBuffer(const Core &core, std::unique_ptr<ShaderObjectManager> &&manager) : BufferObject(
        core, std::move(manager), VK_BUFFER_USAGE_INDEX_BUFFER_BIT) {
        IndexBuffer::attach();
    }

    IndexBuffer::~IndexBuffer() {
        IndexBuffer::detach();
    }

    void IndexBuffer::bind(const VkCommandBuffer cbh, const uint32_t frameIndex, const std::string &varname) const {
        vkCmdBindIndexBuffer(cbh, getBufferHandle(frameIndex), getOffset(varname), VK_INDEX_TYPE_UINT32);
    }


    void IndexBuffer::attach() {
        //no operation
    }

    void IndexBuffer::detach() {
        //no operation
    }
}
