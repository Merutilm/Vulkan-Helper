//
// Created by Merutilm on 2026-02-03.
//
#include "DemoApplication.hpp"

#include "GPCSample.hpp"
#include "PerlinNoise.hpp"
#include "SampleRCC.hpp"
#include "vulkan_helper/util/SwapchainUtils.hpp"


namespace merutilm::mvk_demo {

    DemoApplication::DemoApplication() { DemoApplication::init(); }

    void DemoApplication::init() {
        createWindow();
        addListeners();
        createContexts();
        createRenderers();
    }

    void DemoApplication::createWindow() { wc = &engine.attachWindowContext(mvk::WindowInitializerSettings{}, 0); }

    void DemoApplication::addListeners() {
        wc->getWindow()->eventSystem.mouse.onMouseEnter.addListener([] { std::cout << "Mouse entered" << std::endl; });
        wc->getWindow()->eventSystem.mouse.onMouseExit.addListener([] { std::cout << "Mouse exited" << std::endl; });
        wc->getWindow()->eventSystem.mouseDrag.onMouseDrag.addListener(
                [](mvk::KeyInput::MouseButton, int x, int y) { mvk::logger::log("mouse drag={}, {}", x, y); });
        wc->getWindow()->eventSystem.mouse.onMouseDown.addListener(
                [](mvk::KeyInput::MouseButton, int x, int y) { mvk::logger::log("mouse down={}, {}", x, y); });
        wc->getWindow()->eventSystem.mouse.onMouseUp.addListener(
                [](mvk::KeyInput::MouseButton, int x, int y) { mvk::logger::log("mouse up={}, {}", x, y); });
        wc->getWindow()->eventSystem.keyboard.onKeyDown.addListener(
                [](mvk::KeyInput::Key key) { mvk::logger::log("key={0}", static_cast<int>(key)); });

        wc->getWindow()->eventSystem.applicationLifecycle.onUpdate.addListener([this] { update(); });

    }

    void DemoApplication::createContexts() const {

        wc->attachRenderContext<SampleRCC>(
                [this] { return wc->getSwapchain().populateSwapchainExtent(); },
                [this] { return mvk::ImageContext::fromSwapchain(wc->core, wc->getSwapchain()); });

        wc->getWindow()->eventSystem.resize.onResizeEnd.addListener([this](int, int) {
            if (wc->getWindow()->canRenderable()) {
                wc->core.getLogicalDevice().waitDeviceIdle();
                wc->getSwapchain().recreate();
                wc->getRenderContext(SampleRCC::CONTEXT_INDEX).recreate();
            }
        });

        wc->getWindow()->eventSystem.applicationLifecycle.onQuit.addListener([this] {
            wc->core.getLogicalDevice().waitDeviceIdle();
            mvk::GeneralPostProcessGraphicsPipelineConfigurator::cleanup();
        });
    }
    void DemoApplication::createRenderers() { renderer.emplace(engine, *wc); }

    void DemoApplication::update() { renderer->execute(); }

    void DemoApplication::start() const { wc->getWindow()->start(); }

} // namespace merutilm
