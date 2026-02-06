//
// Created by Merutilm on 2026-02-06.
//

#pragma once
#include <string>
#include <vector>

#include "Listener.hpp"

namespace merutilm::vkh {

    struct EventSystem final {

        enum class MouseButton { LEFT, RIGHT, MIDDLE };

        struct MouseEvents {
            Listener<> onMouseEnter;
            Listener<> onMouseExit;
            Listener<MouseButton, int, int> onMouseDown;
            Listener<MouseButton, int, int> onMouseUp;
            Listener<int, int> onMouseMove;
        };

        struct MouseWheelEvents {
            Listener<float> onMouseScroll;
        };

        struct MouseDragEvents {
            Listener<MouseButton, int, int> onMouseDragStart;
            Listener<MouseButton, int, int> onMouseDrag;
            Listener<MouseButton, int, int> onMouseDragEnd;
        };

        struct KeyboardEvents {
            Listener<int> onKeyDown;
            Listener<int> onKeyUp;
        };

        struct FocusEvents {
            Listener<> onFocusGained;
            Listener<> onFocusLost;
        };

        struct ResizeEvents {
            Listener<int, int> onResizeBegin;
            Listener<int, int> onResizing;
            Listener<int, int> onResizeEnd;
        };

        struct WindowEvents {
            Listener<> onMinimize;
            Listener<> onMaximize;
            Listener<> onClose;
        };

        struct ApplicationLifecycleEvents {
            Listener<> onStart;
            Listener<> onUpdate;
        };

        struct DnDEvents {
            Listener<const std::vector<std::string> &> onFileDrop;
        };

        MouseEvents mouseEvents;
        MouseWheelEvents mouseWheelEvents;
        MouseDragEvents mouseDragEvents;
        KeyboardEvents keyboardEvents;
        FocusEvents focusEvents;
        ResizeEvents resizeEvents;
        WindowEvents windowEvents;
        ApplicationLifecycleEvents applicationLifecycleEvents;
        DnDEvents dnDEvents;


    };
} // namespace merutilm::vkh
