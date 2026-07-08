//
// Created by Merutilm on 7/9/26.
//

#pragma once
#include <vulkan_helper/engine/manage/RenderPassManager.hpp>

namespace merutilm::vkh {


    class RenderPass;

    struct RenderPassGraphGeneratorBase {
        std::unique_ptr<RenderPassManager> rpm;

        RenderPassGraphGeneratorBase() : rpm(std::make_unique<RenderPassManager>()) {}
        virtual ~RenderPassGraphGeneratorBase() = default;


        std::unique_ptr<RenderPassManager> regenerate() {
            rpm = std::make_unique<RenderPassManager>();
            configure();
            generate();
            return std::move(rpm);
        }

        virtual void createPipelines(RenderPass *rp) const = 0;

    protected:
        virtual void configure() = 0;

        virtual void generate() = 0;

    };

} // namespace merutilm::vkh
