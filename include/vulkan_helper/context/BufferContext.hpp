//
// Created by Merutilm on 2025-09-05.
//

#pragma once
#include <vulkan_helper/base/vkh_base.hpp>
#include <vulkan_helper/core/Core.hpp>
#include <vulkan_helper/wrapped/BufferInitInfo.hpp>
#include <vulkan_helper/util/BufferImageUtils.hpp>
#include <vulkan_helper/base/allocator.hpp>

namespace merutilm::vkh {
    struct BufferContext;

    using MultiframeBufferContext = std::vector<BufferContext>;

    struct BufferContext {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
        VkDeviceSize bufferSize;
        std::byte *mappedMemory = nullptr;

        static BufferContext createContext(CoreRef core, const BufferInitInfo &bufferInitInfo);

        static MultiframeBufferContext createMultiframeContext(CoreRef core, const BufferInitInfo &bufferInitInfo);

        static void mapMemory(CoreRef core, MultiframeBufferContext &context);


        static void mapMemory(CoreRef core, BufferContext &context);

        static void unmapMemory(CoreRef core, const MultiframeBufferContext &context);

        static void unmapMemory(CoreRef core, const BufferContext &context);

        template<typename T>
        static void fill(const BufferContext &bufCtx, const std::vector<T> &vec);

        static void flush(const VkDevice device, const BufferContext &bufCtx);

        static void fillZero(const BufferContext &bufCtx);

        template<typename T>
        static void set(const BufferContext &context, const uint32_t index, const T &value);


        template<typename T>
        static T get(const BufferContext &context, const uint32_t index);

        static std::vector<std::byte> getRaw(const BufferContext &context, const uint32_t offset, const uint32_t size);

        static void destroyContext(CoreRef core, const MultiframeBufferContext &bufCtx);


        static void destroyContext(CoreRef core, const BufferContext &bufCtx);
    };
}
