//
// Created by Merutilm on 2025-07-18.
//

#include "Sampler2D.hpp"

#include "../util/ImageContextUtils.hpp"

namespace merutilm::vkh {
    Sampler2D::Sampler2D(const Core &core) : CoreHandler(core) {
        Sampler2D::init();
    }


    Sampler2D::~Sampler2D() {
        Sampler2D::destroy();
    }


    const ImageContext &Sampler2D::getImageContext() const {
        if (!initialized) {
            throw exception_invalid_state{"Sampler2D is not initialized. Is setImageContext() called?"};
        }
        return imageContext;
    }


    void Sampler2D::init() {
        const VkSamplerCreateInfo samplerInfo = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .mipLodBias = 0,
            .anisotropyEnable = VK_TRUE,
            .maxAnisotropy = core.getPhysicalDevice().getPhysicalDeviceProperties().limits.maxSamplerAnisotropy,
            .compareEnable = VK_TRUE,
            .compareOp = VK_COMPARE_OP_LESS,
            .minLod = 0,
            .maxLod = 1,
            .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE
        };
        if (vkCreateSampler(core.getLogicalDevice().getLogicalDeviceHandle(), &samplerInfo, nullptr, &sampler)) {
            throw exception_init("Failed to create sampler!");
        }
    }

    void Sampler2D::destroy() {
        const VkDevice device = core.getLogicalDevice().getLogicalDeviceHandle();
        vkDestroySampler(device, sampler, nullptr);
        if (isUnique) {
            vkDestroyImageView(device, imageContext.imageView, nullptr);
            vkDestroyImage(device, imageContext.image, nullptr);
            vkFreeMemory(device, imageContext.imageMemory, nullptr);
        }
    }
}
