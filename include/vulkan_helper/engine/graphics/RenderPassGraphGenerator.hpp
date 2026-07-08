//
// Created by Merutilm on 7/7/26.
//

#pragma once

#include <queue>
#include <ranges>

#include "RenderPassGraphGeneratorBase.hpp"
#include "vulkan_helper/engine/pipeline/node/GraphicsPipelineNode.hpp"
namespace merutilm::vkh {
    struct RenderPassGraphGenerator : RenderPassGraphGeneratorBase {

        Engine &engine;
        WindowContext &wc;
        std::function<MultiframeImageContext()> swapchainImageContextGetter;

        std::vector<std::unique_ptr<PipelineConfigurator>> pipelineConfigurators{};
        std::vector<std::unique_ptr<GraphicsPipelineNode>> pipelines{};
        uint32_t createdSubpassesCount = 0;

        template<typename F>
            requires std::is_invocable_r_v<MultiframeImageContext, F>
        explicit RenderPassGraphGenerator(Engine &engine, WindowContext &wc, F &&swapchainImageContextGetter) :
            engine(engine), wc(wc), swapchainImageContextGetter(std::forward<F>(swapchainImageContextGetter)) {}

        ~RenderPassGraphGenerator() override = default;
        RenderPassGraphGenerator(const RenderPassGraphGenerator &) = delete;
        RenderPassGraphGenerator &operator=(const RenderPassGraphGenerator &) = delete;
        RenderPassGraphGenerator(RenderPassGraphGenerator &&) = delete;
        RenderPassGraphGenerator &operator=(RenderPassGraphGenerator &&) = delete;


        RenderPassAttachment &appendAttachment(const VkAttachmentDescription &attachmentDescription,
                                               const MultiframeImageContext &imageContext) const {
            return rpm->appendAttachment(attachmentDescription, imageContext);
        }


        void createPipelines(RenderPass *rp) const override {
            for (auto &node: pipelines) {
                node->getPipelineConfigurator().configure(rp, node->getSubpass());
            }
        }

    protected:
        template<typename Tp>
            requires std::is_base_of_v<GraphicsPipelineConfigurator, Tp>
        GraphicsPipelineNode *linkPipeline(std::vector<GraphicsPipelineNode *> &&depends,
                                           RenderPassAttachmentReference &&reference) {
            uint32_t subpassIndex = 0;

            if (depends.empty()) {
                subpassIndex = 0;
            } else {
                for (const auto depend: depends) {
                    subpassIndex = std::max(subpassIndex, depend->getSubpass());
                }
                ++subpassIndex;
            }

            if (subpassIndex >= createdSubpassesCount) { // it is not a while loop.
                rpm->appendSubpass();
                ++createdSubpassesCount;
            }
            auto ptr = dynamic_cast<GraphicsPipelineConfigurator *>(pipelineConfigurators.emplace_back(
                    std::make_unique<Tp>(engine.getCore(), engine.getSharedResource(), engine.getGlobalRepositories(),
                                         wc.getWindowLocalRepositories())
                            ).get());

            return pipelines
                    .emplace_back(std::make_unique<GraphicsPipelineNode>(std::move(depends), ptr, std::move(reference),
                                                                         subpassIndex))
                    .get();
        }


        void generate() override {
            if (pipelines.empty())
                return;

            // auto-generate render pass graph. (Topological sort)
            // find subpass 0
            std::unordered_map<const GraphicsPipelineNode *, uint32_t> referenceCount;
            std::queue<const GraphicsPipelineNode *> queue;

            for (auto &pipeline: pipelines) {
                if (pipeline->getSubpass() == 0) {
                    assert(pipeline->getDepends().empty()); // it should not be happened

                    queue.push(pipeline.get());
                }
                referenceCount.emplace(pipeline.get(), pipeline->getDepends().size());
            }

            std::unordered_map<const RenderPassAttachment *, std::unordered_set<uint32_t>> usedSubpasses;
            std::unordered_map<uint32_t, std::unordered_map<uint32_t, VkSubpassDependency>> generatedDependencies;

            while (!queue.empty()) {
                const GraphicsPipelineNode *node = queue.front();
                queue.pop();
                processNode(usedSubpasses, generatedDependencies, node);
                for (const auto next: node->getNext()) {
                    --referenceCount[next];

                    if (referenceCount[next] == 0) {
                        queue.push(next);
                    }
                }
            }

            for (auto &[attachment, subpasses]: usedSubpasses) {
                if (subpasses.empty())
                    continue;

                std::vector<uint32_t> sorted{subpasses.begin(), subpasses.end()};
                std::ranges::sort(sorted);
                uint32_t prevSp = sorted[0];
                for (size_t i = 1; i < sorted.size(); ++i) {
                    const uint32_t sp = sorted[i];

                    for (uint32_t j = prevSp + 1; j <= sp - 1; ++j) {
                        rpm->setPreserved(*attachment, j);
                    }
                    prevSp = sp;
                }
            }

            for (const auto &val: generatedDependencies | std::views::values) {
                for (const auto &val2: val | std::views::values) {
                    rpm->appendDependency(val2);
                }
            }
        }

    private:
        void processNode(
                std::unordered_map<const RenderPassAttachment *, std::unordered_set<uint32_t>> &usedSubpasses,
                std::unordered_map<uint32_t, std::unordered_map<uint32_t, VkSubpassDependency>> &generatedDependencies,
                const GraphicsPipelineNode *node) const {

            rpm->appendReference(node->getAttachmentReference(), node->getSubpass());

            const uint32_t currentSubpass = node->getSubpass();
            const RenderPassAttachment *attachment = node->getAttachmentReference().targetAttachment;

            std::unordered_set<uint32_t> &usedSubpass =
                    usedSubpasses.try_emplace(attachment, std::unordered_set<uint32_t>{}).first->second;
            usedSubpass.insert(currentSubpass);

            const std::vector<GraphicsPipelineNode *> &depends = node->getDepends();


            for (const auto depend: depends) {
                const uint32_t dependSubpass = depend->getSubpass();

                VkSubpassDependency &dep = generatedDependencies[dependSubpass][currentSubpass];
                dep.srcSubpass = dependSubpass;
                dep.dstSubpass = currentSubpass;
                dep.srcStageMask |= depend->getAttachmentReference().pipelineStageFlags;
                dep.dstStageMask |= node->getAttachmentReference().pipelineStageFlags;
                dep.srcAccessMask |= depend->getAttachmentReference().accessFlags;
                dep.dstAccessMask |= node->getAttachmentReference().accessFlags;
                dep.dependencyFlags |= node->getAttachmentReference().dependencyFlags;
            }
        }
    };
} // namespace merutilm::vkh
