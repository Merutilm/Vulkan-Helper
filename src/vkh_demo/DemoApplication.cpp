//
// Created by Merutilm on 2026-02-03.
//
#include "DemoApplication.hpp"

#include "GPCSample.hpp"
#include "PerlinNoise.hpp"
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
        vkh::PlatformWindowBase &wb = *wc->getWindow();
        auto &menu = wb.addMenu(*wb.menubar, L"File");
        wb.addMenuItem(menu, L"d1");
        wb.addMenuItem(menu, L"d2");
        wb.addMenuItem(menu, L"d3");
        auto &sub = wb.addMenu(menu, L"d4");
        wb.addMenuItem(sub, L"d41");
        wb.addCheckboxMenuItem(sub, L"d42", false);

        auto &cb = wb.addCheckboxMenuItem(menu, L"d5", true);
        cb.onClick.add([](const bool v) { std::cout << v << std::endl; });
    }

    void DemoApplication::addListeners() {
        wc->getWindow()->eventSystem.mouse.onMouseEnter.add([] { std::cout << "Mouse entered" << std::endl; });
        wc->getWindow()->eventSystem.mouse.onMouseExit.add([] { std::cout << "Mouse exited" << std::endl; });
        wc->getWindow()->eventSystem.mouseDrag.onMouseDrag.add(
                [](vkh::KeyInput::MouseButton, int x, int y) { vkh::logger::log("mouse drag={}, {}", x, y); });
        wc->getWindow()->eventSystem.mouse.onMouseDown.add(
                [](vkh::KeyInput::MouseButton, int x, int y) { vkh::logger::log("mouse down={}, {}", x, y); });
        wc->getWindow()->eventSystem.mouse.onMouseUp.add(
                [](vkh::KeyInput::MouseButton, int x, int y) { vkh::logger::log("mouse up={}, {}", x, y); });
        wc->getWindow()->eventSystem.keyboard.onKeyDown.add(
                [](vkh::KeyInput::Key key) { vkh::logger::log("key={0}", static_cast<int>(key)); });

        wc->getWindow()->eventSystem.applicationLifecycle.onUpdate.add([this] { update(); });
    }

    void DemoApplication::createContexts() const {

        wc->attachRenderContext<SampleRCC>(
                [this] { return wc->getSwapchain().populateSwapchainExtent(); },
                [this] { return vkh::ImageContext::fromSwapchain(wc->core, wc->getSwapchain()); });

        wc->getWindow()->eventSystem.resize.onResizeEnd.add([this](int, int) {
            if (wc->getWindow()->canRenderable()) {
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

    void DemoApplication::update() { renderer->execute(); }

    void DemoApplication::start() const { wc->getWindow()->start(); }


} // namespace merutilm::vkh_demo
