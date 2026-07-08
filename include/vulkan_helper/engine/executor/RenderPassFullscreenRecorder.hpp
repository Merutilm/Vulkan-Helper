//
// Created by Merutilm on 2025-07-15.
//

#pragma once
#include <vulkan_helper/engine/configurator/PipelineConfigurator.hpp>

#include "vulkan_helper/engine/configurator/GraphicsPipelineConfigurator.hpp"


namespace merutilm::vkh {
    class RenderPassFullscreenRecorder final : public WindowContextHandler {
        RenderContext &rc;
        const uint32_t frameIndex;
        const uint32_t swapchainImageIndex;

    public:
        explicit RenderPassFullscreenRecorder(WindowContext & wc, RenderContext &rc, uint32_t frameIndex,
                                              uint32_t swapchainImageIndex);

        ~RenderPassFullscreenRecorder() override;

        RenderPassFullscreenRecorder(const RenderPassFullscreenRecorder &) = delete;

        RenderPassFullscreenRecorder &operator=(const RenderPassFullscreenRecorder &) = delete;

        RenderPassFullscreenRecorder(RenderPassFullscreenRecorder &&) = delete;

        RenderPassFullscreenRecorder &operator=(RenderPassFullscreenRecorder &&) = delete;


        static void cmdFullscreenRenderPass(WindowContext & wc, RenderContext &rc, const uint32_t frameIndex, const uint32_t swapchainImageIndex, std::vector<GraphicsPipelineConfigurator *> &&configurators, std::vector<DescIndexPicker> && descIndices) {
            const auto renderPassRecorder = RenderPassFullscreenRecorder(
                wc, rc, frameIndex, swapchainImageIndex);
            renderPassRecorder.execute(frameIndex, std::move(configurators), std::move(descIndices));
        }

        static void cmdFullscreenPresentOnlyRenderPass(WindowContext & wc, RenderContext &rc, const uint32_t frameIndex, const uint32_t swapchainImageIndex, std::vector<GraphicsPipelineConfigurator *> &&configurators, std::vector<DescIndexPicker> && descIndices) {
            cmdFullscreenRenderPass(wc, rc, frameIndex, swapchainImageIndex, std::move(configurators), std::move(descIndices));
        }

        static void cmdFullscreenInternalRenderPass(WindowContext & wc, RenderContext &rc, const uint32_t frameIndex, std::vector<GraphicsPipelineConfigurator *> &&configurators, std::vector<DescIndexPicker> && descIndices) {
            cmdFullscreenRenderPass(wc, rc, frameIndex, UINT32_MAX,  std::move(configurators), std::move(descIndices));
        }



    protected:
        void init() override;

        void cleanup() override;

    private:
        void cmdMatchViewportAndScissor() const;

        void execute(uint32_t frameIndex, std::vector<GraphicsPipelineConfigurator *> &&configurators,
                     std::vector<DescIndexPicker> &&descIndices) const;

    };
}
