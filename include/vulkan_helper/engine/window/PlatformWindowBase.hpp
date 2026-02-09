//
// Created by Merutilm on 2026-02-07.
//

#pragma once
#include "EventSystem.hpp"
#include "MouseState.hpp"
#include "WindowInitializerSettings.hpp"
#include "vulkan_helper/handle/Handler.hpp"

namespace merutilm::vkh {
    class PlatformWindowBase : Handler {

    protected:
        bool controlKeyPressed = false;
        bool altKeyPressed = false;
        bool shiftKeyPressed = false;
        bool mouseHovered;
        bool isResizing;
        MouseState leftMouse = {};
        MouseState middleMouse = {};
        MouseState rightMouse = {};

        std::unordered_map<uint32_t, KeyInput::Key> keyMapper = {};
        std::unordered_set<KeyInput::Key> pressedKeys = {};

        static constexpr int MOUSE_DRAG_THRESHOLD = 10;

        int wheelMoved = 0;

    public:
        EventSystem eventSystem = {};
        WindowInitializerSettings initializerSettings;

        explicit PlatformWindowBase(WindowInitializerSettings &&settings);

        ~PlatformWindowBase() override;
        PlatformWindowBase(const PlatformWindowBase &) = delete;
        PlatformWindowBase &operator=(const PlatformWindowBase &) = delete;
        PlatformWindowBase(PlatformWindowBase &&) = delete;
        PlatformWindowBase &operator=(PlatformWindowBase &&) = delete;

        void init() override;

        virtual void start();

        virtual VkExtent2D calculateClientSizeFromMainWindow() const = 0;

        virtual VkSurfaceKHR createSurface(VkInstance instance) = 0;

        virtual KeyInput::Key findEquivalentKey(uint32_t nativeKey);

        virtual void generateKeyMapper() = 0;

        virtual bool canRenderable() = 0;

        bool isKeyPressed(KeyInput::Key key) const;

        bool isControlKeyPressed() const { return controlKeyPressed; }
        bool isAltKeyPressed() const { return altKeyPressed; }
        bool isShiftKeyPressed() const { return shiftKeyPressed; }
        const MouseState &getLeftMouseState() const { return leftMouse; }
        const MouseState &getMiddleMouseState() const { return middleMouse; }
        const MouseState &getRightMouseState() const { return rightMouse; }

        void cleanup() override;
    };
} // namespace merutilm::vkh
