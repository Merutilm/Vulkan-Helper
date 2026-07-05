//
// Created by Merutilm on 2026-02-03.
//
#include "DemoApplication.hpp"

#include "GPCSample.hpp"
#include "SampleRCC.hpp"
#include "vulkan_helper/util/SwapchainUtils.hpp"


namespace merutilm::vkh_demo {

    DemoApplication::DemoApplication() { DemoApplication::init(); }

    void DemoApplication::init() {
        Application::init();
        createWindow();
        initMenu();
        addListeners();
        createContexts();
        createRenderers();
    }

    void DemoApplication::cleanup() { Application::cleanup(); }

    void DemoApplication::createWindow() {
        wc = &engine->attachWindowContext(vkh::WindowInitializerSettings{.useMenubar = true}, 0);
    }

    void DemoApplication::initMenu() {
        vkh::PlatformWindow &wb = *wc->getWindow();
        // auto &menu = wb.addMenu(*wb.menubar, "File");
        // wb.addMenuItem(menu, "d1");
        // wb.addMenuItem(menu, "d2");
        // wb.addMenuItem(menu, "d3");
        // auto &sub = wb.addMenu(menu, "d4");
        // wb.addMenuItem(sub, "d41");
    }

    void DemoApplication::addListeners() {
        wc->getWindow()->eventSystem.mouse.onMouseEnter.add([] { std::cout << "Mouse entered" << std::endl; });
        wc->getWindow()->eventSystem.mouse.onMouseExit.add([] { std::cout << "Mouse exited" << std::endl; });
        wc->getWindow()->eventSystem.mouseDrag.onMouseDrag.add(
                [](int, int x, int y, int , int ) { vkh::logger::log("mouse drag={}, {}", x, y); });
        wc->getWindow()->eventSystem.mouse.onMouseDown.add(
                [](int, int x, int y) { vkh::logger::log("mouse down={}, {}", x, y); });
        wc->getWindow()->eventSystem.mouse.onMouseUp.add(
                [](int, int x, int y) { vkh::logger::log("mouse up={}, {}", x, y); });
        wc->getWindow()->eventSystem.keyboard.onKeyDown.add(
                [](int key) { vkh::logger::log("key={0}", key); });

        wc->getWindow()->eventSystem.applicationLifecycle.onUpdate.add([this] { update(); });
    }

    void DemoApplication::createContexts() const {

        wc->attachRenderContext<SampleRCC>(
                [this] { return wc->getSwapchain().getSwapchainExtent(); },
                [this] { return vkh::ImageContext::fromSwapchain(wc->core, wc->getSwapchain()); });

        wc->getWindow()->eventSystem.resize.onResize.add([this] {
            if (wc->getWindow()->lastExtentUpdate()) {
                wc->core.getLogicalDevice().waitDeviceIdle();
                wc->getSwapchain().recreate();
                wc->getRenderContext(SampleRCC::CONTEXT_INDEX).recreate();
            }
        });

        wc->getWindow()->eventSystem.applicationLifecycle.onQuit.add(
                [this] { wc->core.getLogicalDevice().waitDeviceIdle(); });
    }
    void DemoApplication::createRenderers() {
        sharedResource.emplace(*wc);
        renderer.emplace(*engine, *wc, *sharedResource->vertexBufferPP, *sharedResource->indexBufferPP);
    }

    void DemoApplication::update() {

        renderer->execute();
    }

    void DemoApplication::start() const { wc->getWindow()->start(); }


} // namespace merutilm::vkh_demo
