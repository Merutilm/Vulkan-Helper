//
// Created by Merutilm on 2025-09-03.
//

#pragma once
#include <vulkan_helper/base/vkh_base.hpp>
#include <vulkan_helper/context/ImageContext.hpp>

namespace merutilm::vkh {
    class SharedImageContextImpl final {
        CoreRef core;
        std::vector<MultiframeImageContext> multiframeContexts = {};
        std::vector<ImageContext> imageContexts = {};

    public:
        explicit SharedImageContextImpl(CoreRef core);

        ~SharedImageContextImpl();

        SharedImageContextImpl(const SharedImageContextImpl &) = delete;

        SharedImageContextImpl &operator=(const SharedImageContextImpl &) = delete;

        SharedImageContextImpl(SharedImageContextImpl &&) = delete;

        SharedImageContextImpl &operator=(SharedImageContextImpl &&) = delete;

        void appendMultiframeImageContext(const uint32_t expected, ImageInitInfo &&iii);

        void appendImageContext(const uint32_t expected, ImageInitInfo &&iii);

        MultiframeImageContext &getImageContextMF(const uint32_t index) {
            return multiframeContexts[index];
        }

        ImageContext &getImageContext(const uint32_t index) {
            return imageContexts[index];
        }

        void cleanupContexts();
    };

    using SharedImageContext = std::unique_ptr<SharedImageContextImpl>;
    using SharedImageContextPtr = SharedImageContextImpl *;
    using SharedImageContextRef = SharedImageContextImpl &;
}
