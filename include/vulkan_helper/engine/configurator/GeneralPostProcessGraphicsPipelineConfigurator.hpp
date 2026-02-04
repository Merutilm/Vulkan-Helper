//
// Created by Merutilm on 2025-08-05.
//

#pragma once
#include <vulkan_helper/engine/buffer/IndexBuffer.hpp>
#include <vulkan_helper/engine/buffer/VertexBuffer.hpp>
#include "GeneralGraphicsPipelineConfigurator.hpp"
#include "GraphicsPipelineConfigurator.hpp"

namespace merutilm::vkh {
    class GeneralPostProcessGraphicsPipelineConfigurator : public GraphicsPipelineConfigurator {
        inline static bool initializedVertexIndex = false;
        inline static std::optional<VertexBuffer> vertexBufferPP;
        inline static std::optional<IndexBuffer> indexBufferPP;
        static constexpr auto VERTEX_MODULE_PATH = "vk_post_process.vert";

    public:

        explicit GeneralPostProcessGraphicsPipelineConfigurator(Engine & engine, const uint32_t windowContextIndex, const uint32_t renderContextIndex, const uint32_t primarySubpassIndex, const std::string &fragName) : GraphicsPipelineConfigurator(
            engine, windowContextIndex, renderContextIndex, primarySubpassIndex, VERTEX_MODULE_PATH, fragName) {
        }
        ~GeneralPostProcessGraphicsPipelineConfigurator() override = default;

        GeneralPostProcessGraphicsPipelineConfigurator(const GeneralPostProcessGraphicsPipelineConfigurator &) = delete;

        GeneralPostProcessGraphicsPipelineConfigurator &operator=(const GeneralPostProcessGraphicsPipelineConfigurator &) = delete;

        GeneralPostProcessGraphicsPipelineConfigurator(GeneralPostProcessGraphicsPipelineConfigurator &&) noexcept = default;

        GeneralPostProcessGraphicsPipelineConfigurator &operator=(GeneralPostProcessGraphicsPipelineConfigurator &&) = delete;


        void configure() override;

        void cmdRender(VkCommandBuffer cbh, uint32_t frameIndex, DescIndexPicker &&descIndices) override;

        static void cleanup() {
            vertexBufferPP = std::nullopt;
            indexBufferPP = std::nullopt;
            initializedVertexIndex = false;
        }

    protected:
        [[nodiscard]] VertexBuffer & getVertexBuffer() override { return *vertexBufferPP; }

        [[nodiscard]] IndexBuffer & getIndexBuffer() override { return *indexBufferPP; }

        void configureVertexBuffer(HostDataObjectManager & som) override;

        void configureIndexBuffer(HostDataObjectManager & som) override;

    };
}
