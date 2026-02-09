//
// Created by Merutilm on 2026-02-06.
//
#pragma once
namespace merutilm::vkh {

    struct WindowInitializerSettings {

        struct Paddings {
            int top = 0;
            int left = 0;
            int bottom = 0;
            int right = 0;
        };

        float framerate = 60;
        const char * name = "VulkanHelperApplication";
        const char * icon = "";
        int width = 300;
        int height = 300;
        bool useMenubar = false;
        bool resizable = true;
        bool filedrop = false;
        Paddings paddings {};

    };
}