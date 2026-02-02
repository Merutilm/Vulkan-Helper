//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <vulkan_helper/repo/Repository.hpp>
#include <vulkan_helper/hash/PipelineLayoutBuilderHasher.hpp>
#include <vulkan_helper/manage/PipelineLayoutManager.hpp>
#include <vulkan_helper/core/PipelineLayout.hpp>

namespace merutilm::vkh {
    struct GlobalPipelineLayoutRepo final : Repository<PipelineLayoutBuilder, PipelineLayoutManager &&, PipelineLayout,
                PipelineLayoutRef, PipelineLayoutBuilderHasher, std::equal_to<>> {
        using Repository::Repository;

        PipelineLayoutRef pick(PipelineLayoutManager &&layoutManager) override;
    };
}
