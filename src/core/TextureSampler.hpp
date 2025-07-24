//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include "CommandPool.hpp"
#include "../handle/CoreHandler.hpp"

namespace merutilm::vkh {
    class TextureSampler final : public CoreHandler {

        const CommandPool &commandPool;
        const std::string path;

        VkImage image = nullptr;
        VkDeviceMemory memory = nullptr;
        VkImageView view = nullptr;
        VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkSampler sampler = nullptr;

    public:
        explicit TextureSampler(const Core &core, const CommandPool &commandPool, const std::string &path);

        ~TextureSampler() override;

        TextureSampler(const TextureSampler &) = delete;

        TextureSampler &operator=(const TextureSampler &) = delete;

        TextureSampler(TextureSampler &&) = delete;

        TextureSampler &operator=(TextureSampler &&) = delete;

    private:
        void attach() override;

        void detach() override;

        void transformImageLayout(VkCommandBuffer commandBuffer, VkAccessFlags srcAccessMask,
                                  VkAccessFlags dstAccessMask,
                                  VkImageLayout newLayout, VkPipelineStageFlags srcStageMask,
                                  VkPipelineStageFlags dstStageMask);
    };
}
