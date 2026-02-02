//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/context/SharedImageContext.hpp>

namespace merutilm::vkh {
    SharedImageContextImpl::SharedImageContextImpl(CoreRef core): core(core) {
    }

    SharedImageContextImpl::~SharedImageContextImpl() {
        cleanupContexts();
    }

    void SharedImageContextImpl::appendMultiframeImageContext(const uint32_t expected, ImageInitInfo &&iii) {
        safe_array::check_index_equal(expected, multiframeContexts.size(),
                                      "Shared Multiframe Image Context append");
        multiframeContexts.emplace_back(ImageContext::createMultiframeContext(core, iii));
    }

    void SharedImageContextImpl::appendImageContext(const uint32_t expected, ImageInitInfo &&iii) {
        safe_array::check_index_equal(expected, imageContexts.size(), "Shared Image Context append");
        imageContexts.emplace_back(ImageContext::createContext(core, iii));
    }

    void SharedImageContextImpl::cleanupContexts() {
        for (const auto &context: multiframeContexts) {
            ImageContext::destroyContext(core, context);
        }
        for (const auto &context: imageContexts) {
            ImageContext::destroyContext(core, context);
        }
        multiframeContexts.clear();
        imageContexts.clear();
    }
}
