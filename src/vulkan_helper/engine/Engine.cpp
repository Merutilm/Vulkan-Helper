//
// Created by Merutilm on 2025-07-19.
//

#include <vulkan_helper/engine/Engine.hpp>

#include <vulkan_helper/engine/repo/GlobalDescriptorSetLayoutRepo.hpp>
#include <vulkan_helper/engine/repo/GlobalPipelineLayoutRepo.hpp>
#include <vulkan_helper/engine/repo/GlobalSamplerRepo.hpp>

namespace merutilm::vkh {
    Engine::Engine() {
        Engine::init();
    }

    Engine::~Engine() {
        Engine::cleanup();
    }

    bool Engine::isValidWindowContext(const uint32_t windowAttachmentIndex) const {
        return windowContexts.size() > windowAttachmentIndex && windowContexts[windowAttachmentIndex] != std::nullopt;
    }

    WindowContext & Engine::attachWindowContext(HWND hwnd, uint32_t windowAttachmentIndexExpected) {


        if (windowAttachmentIndexExpected >= windowContexts.size()) {
            windowContexts.resize(windowAttachmentIndexExpected + 1);
        }
        if (windowContexts[windowAttachmentIndexExpected] != std::nullopt) {
            throw exception_invalid_args(std::format("given window context {} is already using", windowAttachmentIndexExpected));
        }

        auto window = GraphicsContextWindow(hwnd);

        windowContexts[windowAttachmentIndexExpected].emplace(core, windowAttachmentIndexExpected, std::move(window));
        return *windowContexts[windowAttachmentIndexExpected];
    }

    void Engine::detachWindowContext(const uint32_t windowAttachmentIndex) {
        if (windowAttachmentIndex >= windowContexts.size()) {
            throw exception_invalid_args(std::format("window attachment index out of range : {}, size = {}", windowAttachmentIndex, windowContexts.size()));
        }
        if (windowContexts[windowAttachmentIndex] == std::nullopt) {
            throw exception_invalid_args(std::format("deleted non-existing context {}", windowAttachmentIndex));
        }
        windowContexts[windowAttachmentIndex].reset();
    }


    void Engine::init() {
        configureRepositories();
    }

    void Engine::configureRepositories() {
        globalRepositories.addRepository<GlobalDescriptorSetLayoutRepo>(core);
        globalRepositories.addRepository<GlobalPipelineLayoutRepo>(core);
        globalRepositories.addRepository<GlobalShaderModuleRepo>(core);
        globalRepositories.addRepository<GlobalSamplerRepo>(core);
    }


    void Engine::cleanup() {
        windowContexts.clear();
        globalRepositories.clear();
    }
}
