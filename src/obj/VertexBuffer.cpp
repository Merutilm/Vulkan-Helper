//
// Created by Merutilm on 2025-07-15.
//

#include "VertexBuffer.hpp"

#include "../manage/BufferObjectManager.hpp"
#include "../struct/Vertex.hpp"
#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    VertexBuffer::VertexBuffer(const Core &core, std::unique_ptr<BufferObjectManager> &&manager) : BufferObject(
        core, std::move(manager), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {
        VertexBuffer::attach();
    }

    VertexBuffer::~VertexBuffer() {
        VertexBuffer::detach();
    }


    void VertexBuffer::attach() {
        const uint32_t len = getObjectCount();
        for (uint32_t i = 0; i < len; ++i) {
            const uint32_t size = sizes[i];
            const uint32_t offset = offsets[i];
            if (const uint32_t element = elements[i]; size != sizeof(Vertex) * element) {
                throw exception_invalid_args(std::format("size {} and {} is not match", size,
                                                         sizeof(Vertex) * element));
            }
            bindingDescriptions.emplace_back(i, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX);
            vertexInputAttributeDescriptions.emplace_back(0, i, getFormat<decltype(Vertex::position)>(),
                                                          offset + offsetof(Vertex, position));
            vertexInputAttributeDescriptions.emplace_back(1, i, getFormat<decltype(Vertex::color)>(),
                                                          offset + offsetof(Vertex, color));
        }
    }

    void VertexBuffer::detach() {
        //nothing to do
    }
}
