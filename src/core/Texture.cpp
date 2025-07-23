//
// Created by Merutilm on 2025-07-18.
//

#include "Texture.hpp"
#include <stb_image.h>

#include "../executor/ScopedCommandExecutor.hpp"

namespace merutilm::vkh {
    Texture::Texture(const Engine &engine, const std::string &path) : EngineHandler(engine), path(path) {
        Texture::attach();
    }

    Texture::~Texture() {
        Texture::detach();
    }

    void Texture::attach() {
        stbi_uc *data = nullptr;
        int width = 0;
        int height = 0;
        int texChannels = 0;
        data = stbi_load(path.data(), &width, &height, &texChannels, STBI_rgb_alpha);
        if (data == nullptr) {
            throw exception_init("Failed to load texture");
        }

        VkBuffer stagingBuffer = nullptr;
        VkDeviceMemory stagingMemory = nullptr;
        const VkDevice device = engine.getCore().getLogicalDevice().getLogicalDeviceHandle();
        const VkDeviceSize size = static_cast<uint64_t>(width) * static_cast<uint64_t>(height) * static_cast<uint64_t>(
                                      texChannels);

        BufferUtils::initBuffer(engine.getCore(), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                &stagingBuffer, &stagingMemory);
        void *mapped = nullptr;
        vkMapMemory(device, stagingMemory, 0, size, 0, &mapped);
        memcpy(mapped, data, size);
        vkUnmapMemory(device, stagingMemory);


        const ImageInitInfo initInfo = {
            .imageType = VK_IMAGE_TYPE_2D,
            .imageViewType = VK_IMAGE_VIEW_TYPE_2D,
            .imageFormat = VK_FORMAT_R8G8B8A8_SRGB,
            .extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1},
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .imageTiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_SAMPLED_BIT,
            .initialLayout = currentLayout,
            .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        BufferUtils::initImage(engine.getCore(), initInfo, &image, &memory, &view);


        stbi_image_free(data);
        //COMMAND START
        {
            const auto sce = ScopedCommandExecutor(engine.getCore(), engine.getCommandPool());
            const VkBufferImageCopy copyRegion = {
                .bufferOffset = 0,
                .bufferRowLength = 0,
                .bufferImageHeight = 0,
                .imageSubresource = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .mipLevel = 0,
                    .baseArrayLayer = 0,
                    .layerCount = 1

                },
                .imageOffset = {0, 0, 0},
                .imageExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1}
            };
            transformImageLayout(sce.getCommandBufferHandle(), VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT,
                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_HOST_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT);

            vkCmdCopyBufferToImage(sce.getCommandBufferHandle(), stagingBuffer, image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
            transformImageLayout(sce.getCommandBufferHandle(), VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
                                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
        }
        //COMMAND END

        vkFreeMemory(device, stagingMemory, nullptr);
        vkDestroyBuffer(device, stagingBuffer, nullptr);


        VkSamplerCreateInfo samplerInfo = {
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
            .maxAnisotropy = engine.getCore().getPhysicalDevice().getPhysicalDeviceProperties().limits.maxSamplerAnisotropy,
            .compareEnable = VK_TRUE,
            .compareOp = VK_COMPARE_OP_LESS,
            .minLod = 0,
            .maxLod = 1,
            .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE
        };
        if (vkCreateSampler(engine.getCore().getLogicalDevice().getLogicalDeviceHandle(), &samplerInfo, nullptr, &sampler)) {
            throw exception_init("Failed to create sampler!");
        }

    }

    void Texture::detach() {
        const VkDevice device = engine.getCore().getLogicalDevice().getLogicalDeviceHandle();
        vkDestroySampler(device, sampler, nullptr);
        vkDestroyImageView(device, view, nullptr);
        vkFreeMemory(device, memory, nullptr);
        vkDestroyImage(device, image, nullptr);
    }


    void Texture::transformImageLayout(const VkCommandBuffer commandBuffer, const VkAccessFlags srcAccessMask,
                                       const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkPipelineStageFlags srcStageMask,
                                       const VkPipelineStageFlags dstStageMask) {
        const VkImageMemoryBarrier barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = srcAccessMask,
            .dstAccessMask = dstAccessMask,
            .oldLayout = currentLayout,
            .newLayout = newLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = image,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        vkCmdPipelineBarrier(commandBuffer, srcStageMask,
                             dstStageMask, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);
        currentLayout = newLayout;
    }
}
