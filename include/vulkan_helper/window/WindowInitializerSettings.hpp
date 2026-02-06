//
// Created by Merutilm on 2026-02-06.
//
#pragma once
namespace merutilm::vkh {

    struct WindowInitializerSettings {

        struct Paddings {
            int top;
            int left;
            int bottom;
            int right;
        };

        const char * name;
        const char * icon;
        int width;
        int height;
        bool useMenubar;
        bool resizable;
        Paddings paddings;

    };
}