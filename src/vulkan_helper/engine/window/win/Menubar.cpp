//
// Created by Merutilm on 2026-02-07.
//

#ifdef WIN32
#include <vulkan_helper/engine/window/win/Menubar.hpp>
namespace merutilm::mvk {

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
} // namespace merutilm::mvk
#endif