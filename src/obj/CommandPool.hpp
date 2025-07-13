//
// Created by Merutilm on 2025-07-09.
//

#pragma once
#include "../objhandler/EngineHandler.hpp"

namespace merutilm::vkh {
    class CommandPool final : public EngineHandler {
        std::vector<VkCommandPool> commandPools = {};

    public:
        explicit CommandPool(const Engine &engine);

        ~CommandPool() override;

        CommandPool(const CommandPool &) = delete;

        CommandPool &operator=(const CommandPool &) = delete;

        CommandPool(CommandPool &&) = delete;

        CommandPool &operator=(CommandPool &&) = delete;

        [[nodiscard]] VkCommandPool getCommandPoolHandle(const uint32_t frameIndex) const {
            return commandPools[frameIndex];
        }

    private:
        void init() override;

        void destroy() override;
    };
}
