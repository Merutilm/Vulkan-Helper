//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include "../handle/EngineHandler.hpp"

namespace merutilm::vkh {
    class Texture final : public EngineHandler {

        const std::string path;

        VkImage image = nullptr;
        VkDeviceMemory memory = nullptr;
        VkImageView view = nullptr;
        VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkSampler sampler = nullptr;

    public:
        explicit Texture(const Engine &engine, const std::string &path);

        ~Texture() override;

        Texture(const Texture &) = delete;

        Texture &operator=(const Texture &) = delete;

        Texture(Texture &&) = delete;

        Texture &operator=(Texture &&) = delete;

    private:
        void attach() override;

        void detach() override;

        void transformImageLayout(VkCommandBuffer commandBuffer, VkAccessFlags srcAccessMask,
                                  VkAccessFlags dstAccessMask,
                                  VkImageLayout newLayout, VkPipelineStageFlags srcStageMask,
                                  VkPipelineStageFlags dstStageMask);
    };
}
