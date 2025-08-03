//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include "CommandPool.hpp"
#include "../handle/CoreHandler.hpp"

namespace merutilm::vkh {
    class Sampler2D final : public CoreHandler {
        const CommandPool &commandPool;
        const std::string path;

        VkImage image = nullptr;
        VkDeviceMemory memory = nullptr;
        VkImageView view = nullptr;
        VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkSampler sampler = nullptr;

    public:
        explicit Sampler2D(const Core &core, const CommandPool &commandPool, std::string path);

        ~Sampler2D() override;

        Sampler2D(const Sampler2D &) = delete;

        Sampler2D &operator=(const Sampler2D &) = delete;

        Sampler2D(Sampler2D &&) = delete;

        Sampler2D &operator=(Sampler2D &&) = delete;

        [[nodiscard]] VkImage getImageHandle() const { return image; }

        [[nodiscard]] VkImageView getImageViewHandle() const { return view; }

        [[nodiscard]] VkImageLayout getCurrentImageLayout() const { return currentLayout; }

        [[nodiscard]] VkSampler getSamplerHandle() const { return sampler; }

    private:
        void init() override;

        void destroy() override;

        void transformImageLayout(VkCommandBuffer commandBuffer, VkAccessFlags srcAccessMask,
                                  VkAccessFlags dstAccessMask,
                                  VkImageLayout newLayout, VkPipelineStageFlags srcStageMask,
                                  VkPipelineStageFlags dstStageMask);
    };
}
