//
// Created by Merutilm on 2026-02-06.
//

#pragma once


#include "handle/EngineHandler.hpp"
namespace merutilm::vkh {


    struct Application : Handler {

        std::optional<Engine> engine = std::nullopt;
        WindowContext *rootWindowContext = nullptr;
        WindowInitializerSettings rootWindowInitializerSettings;

        static constexpr int WC_ROOT = 0;

        explicit Application(WindowInitializerSettings rootWindowInitializerSettings);

        ~Application() override;

        Application(const Application &) = delete;

        Application(Application &&) = delete;

        Application &operator=(const Application &) = delete;

        Application &operator=(Application &&) = delete;


        template<typename App> requires std::is_base_of_v<Application, App>
        static void start(const WindowInitializerSettings &wic) {
            VkExtent2D extent = {static_cast<uint32_t>(wic.widthInfo.first), static_cast<uint32_t>(wic.heightInfo.first)};
            App app(wic);
            app.refreshSharedImgContexts(extent);
            app.createRenderers();
            app.addListeners();
            app.start();
        }


        virtual void refreshSharedImgContexts(VkExtent2D extent) = 0;

        virtual void createRenderers() = 0;

        virtual void addListeners();

        virtual void update() = 0;

        void start() const {
            rootWindowContext->getWindow()->start();
        }

    protected:
        void init() override;

        void cleanup() override;

#ifdef VKH_USE_IMGUI
        virtual void renderImGui() = 0;

        void createImGuiContext(RenderContext *rc);
#endif
    private:

        void configureRootRenderContexts();

#ifdef VKH_USE_IMGUI
        static void imguiVkResultConsumer(VkResult result);
#endif

    };


} // namespace merutilm::vkh
