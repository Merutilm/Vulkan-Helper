//
// Created by Merutilm on 2026-02-03.
//
#include "DemoApplication.hpp"

#include "GPCSample.hpp"
#include "SampleRCC.hpp"
#include "vulkan_helper/util/SwapchainUtils.hpp"

#ifdef VKH_USE_IMGUI
#include "imgui_impl_vulkan.h"
#endif

namespace merutilm::vkh_demo {


    void DemoApplication::registerRenderers() {
#ifdef VKH_USE_IMGUI
        renderer = registerRenderer<SampleRenderer>(*engine, *rootWindowContext, [this] { renderImGui(); });
        createImGuiContext(renderer->imguiRenderContext);
#else
        renderer = registerRenderer<SampleRenderer>(*engine, *rootWindowContext);
#endif
    }


#ifdef VKH_USE_IMGUI
    void DemoApplication::renderImGui() {
        static float f = 0.0f;
        static int counter = 0;
        static bool a = false;
        static bool b = false;
        static std::array<float, 3> col = {0, 0, 0};
        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &a); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &b);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", col.data()); // Edit 3 floats representing a color

        if (ImGui::Button(
                    "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::End();
    }

    void DemoApplication::onStart() {
        // noop
    }
    void DemoApplication::onQuit() {

    }

#endif
    void DemoApplication::refreshSharedImgContexts(const VkExtent2D extent) {
        rootWindowContext->getSharedImageContext().cleanupContexts();
    }

    void DemoApplication::addListeners() {
        rootWindowContext->getWindow()->eventSystem.applicationLifecycle.onStart.add([this] { onStart(); });
        rootWindowContext->getWindow()->eventSystem.applicationLifecycle.onUpdate.add([this] { update(); });
        rootWindowContext->getWindow()->eventSystem.applicationLifecycle.onQuit.add([this] {
            rootWindowContext->core.getLogicalDevice().waitDeviceIdle();
            onQuit();
        });
        rootWindowContext->getWindow()->eventSystem.resize.onResize.add(
                [this](const uint32_t w, const uint32_t h) { recreateContexts(VkExtent2D{w, h}); });
        rootWindowContext->getWindow()->eventSystem.mouse.onMouseEnter.add(
                [] { std::cout << "Mouse entered" << std::endl; });
        rootWindowContext->getWindow()->eventSystem.mouse.onMouseExit.add(
                [] { std::cout << "Mouse exited" << std::endl; });
        rootWindowContext->getWindow()->eventSystem.mouseDrag.onMouseDrag.add(
                [](int, int x, int y, int, int) { vkh::logger::log("mouse drag={}, {}", x, y); });

        rootWindowContext->getWindow()->eventSystem.mouseWheel.onMouseScroll.add(
                [](int y) { vkh::logger::log("mouse scroll={}", y); });
        rootWindowContext->getWindow()->eventSystem.mouse.onMouseDown.add(
                [](int, int x, int y) { vkh::logger::log("mouse down={}, {}", x, y); });
        rootWindowContext->getWindow()->eventSystem.mouse.onMouseUp.add(
                [](int, int x, int y) { vkh::logger::log("mouse up={}, {}", x, y); });
        rootWindowContext->getWindow()->eventSystem.keyboard.onKeyDown.add(
                [](int key) { vkh::logger::log("key={0}", key); });
    }


    void DemoApplication::update() { renderer->render(); }


} // namespace merutilm::vkh_demo
