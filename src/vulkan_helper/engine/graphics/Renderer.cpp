//
// Created by Merutilm on 2026-02-03.
//
#include <vulkan_helper/util/SwapchainUtils.hpp>
#include <vulkan_helper/engine/graphics/Renderer.hpp>

namespace merutilm::vkh {
    Renderer::Renderer(Engine & engine, const uint32_t windowContextIndex): engine(engine),
        wc(engine.getWindowContext(windowContextIndex)) {
    }

    Renderer::~Renderer() = default;

    uint32_t Renderer::getFrameIndex() const {
        return frameIndex;
    }

    void Renderer::finishPipelineInitialization() {
        for (auto &sp: configurators) {
            sp.pipelineInitialized();
        }
    }

    void Renderer::execute() {
        SwapchainUtils::renderFrame(wc, &frameIndex, [this](const uint32_t swapchainImageIndex) {
            if (frameIndex == 0) {
                for (auto &rc: wc.getRenderContexts()) {
                    rc.getConfigurator()->allFrameInitialized();
                }
            }
            DescriptorUpdateQueue queue = DescriptorUpdater::createQueue();
            const VkDevice device = wc.core.getLogicalDevice().getLogicalDeviceHandle();

            for (auto &configurator: configurators) {
                configurator.updateQueue(queue, frameIndex);
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
