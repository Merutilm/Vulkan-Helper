//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/core/Renderer.hpp>

namespace merutilm::vkh {
    RendererAbstract::RendererAbstract(EngineRef engine, const uint32_t windowContextIndex): engine(engine),
        wc(engine.getWindowContext(windowContextIndex)) {
    }

    RendererAbstract::~RendererAbstract() = default;

    uint32_t RendererAbstract::getFrameIndex() const {
        return frameIndex;
    }

    void RendererAbstract::finishPipelineInitialization() const {
        for (const auto &sp: configurators) {
            sp->pipelineInitialized();
        }
    }

    void RendererAbstract::execute() {
        SwapchainUtils::renderFrame(wc, &frameIndex, [this](const uint32_t swapchainImageIndex) {
            if (frameIndex == 0) {
                for (auto &rc: wc.getRenderContexts()) {
                    rc->getConfigurator()->allFrameInitialized();
                }
            }
            DescriptorUpdateQueue queue = DescriptorUpdater::createQueue();
            const VkDevice device = wc.core.getLogicalDevice().getLogicalDeviceHandle();

            for (const auto &configurator: configurators) {
                configurator->updateQueue(queue, frameIndex);
            }

            DescriptorUpdater::write(device, queue);

            const VkFence fence = wc.getSyncObject().getFence(frameIndex).getFenceHandle();
            const VkSemaphore imageAvailableSemaphore = wc.getSyncObject().getSemaphore(frameIndex).
                    getImageAvailable();
            const VkSemaphore renderFinishedSemaphore = wc.getSyncObject().getSemaphore(frameIndex).
                    getRenderFinished();
            beforeCmdRender();
            ScopedCommandBufferExecutor executor(wc, frameIndex, fence, imageAvailableSemaphore,
                                                 renderFinishedSemaphore);
            cmdRender(swapchainImageIndex);
        });
    }
}
