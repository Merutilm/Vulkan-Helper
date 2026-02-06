//
// Created by Merutilm on 2025-07-19.
//

#pragma once
#include <vulkan_helper/base/pch.hpp>

#include <vulkan_helper/engine/context/WindowContext.hpp>
#include <vulkan_helper/core/Core.hpp>

namespace merutilm::vkh {
    class Engine final : public Handler {
        Core core;
        Repositories globalRepositories;
        std::vector<std::optional<WindowContext>> windowContexts = {};

    public:
        explicit Engine();

        ~Engine() override;

        Engine(const Engine &) = delete;

        Engine &operator=(const Engine &) = delete;

        Engine(Engine &&) noexcept = default;

        Engine &operator=(Engine &&) = delete;

        [[nodiscard]] bool isValidWindowContext(uint32_t windowAttachmentIndex) const;

        [[nodiscard]] WindowContext & attachWindowContext(HWND hwnd, uint32_t windowAttachmentIndexExpected);

        [[nodiscard]] WindowContext detachWindowContext(uint32_t windowAttachmentIndex);

        [[nodiscard]] Core & getCore() { return core; }

        [[nodiscard]] WindowContext & getWindowContext(const uint32_t windowContextIndex)  {
            return *windowContexts.at(windowContextIndex);
        }

        [[nodiscard]] Repositories & getGlobalRepositories()  {
            return globalRepositories;
        }

        [[nodiscard]] uint32_t getWindowContextCount() const { return windowContexts.size(); }

    private:
        void init() override;

        void configureRepositories();

        void cleanup() override;
    };



}
