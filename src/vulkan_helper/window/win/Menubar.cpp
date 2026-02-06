//
// Created by Merutilm on 2026-02-07.
//

#ifdef WIN32
#include <vulkan_helper/window/win/Menubar.hpp>
namespace merutilm::vkh {

    Menubar::Menubar(const HMENU handle) : handle(handle) {
        Menubar::init();
    }
    Menubar::~Menubar() {
        Menubar::cleanup();
    }
    void Menubar::init() {

    }
    void Menubar::cleanup() {

    }
} // namespace merutilm::vkh
#endif