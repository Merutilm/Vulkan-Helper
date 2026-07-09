//
// Created by Merutilm on 7/7/26.
//

#pragma once

#include <vector>

#include "vulkan_helper/engine/configurator/GraphicsPipelineConfigurator.hpp"
#include "vulkan_helper/engine/wrapped/RenderPassAttachmentReference.hpp"

namespace merutilm::vkh {

    class GraphicsPipelineNode {

    protected:
        std::vector<GraphicsPipelineNode *> depends = {};
        std::vector<GraphicsPipelineNode *> next = {};
        GraphicsPipelineConfigurator * configurator = nullptr;
        RenderPassAttachmentReference attachmentReference;
        std::function<DescIndexPicker()> descIndexPickerFunc;
        uint32_t subpass;

    public:

        template<typename Func> requires std::is_invocable_r_v<DescIndexPicker, Func>
        explicit GraphicsPipelineNode(std::vector<GraphicsPipelineNode *> &&depends, GraphicsPipelineConfigurator *configurator, RenderPassAttachmentReference &&attachmentReference, const uint32_t subpass, Func &&descIndexPickerFunc) :
            depends(std::move(depends)), configurator(configurator), attachmentReference(std::move(attachmentReference)), subpass(subpass), descIndexPickerFunc(std::forward<Func>(descIndexPickerFunc)) {
            for (const auto depend: this->depends) {
                depend->next.emplace_back(this);
            }
        }

        virtual ~GraphicsPipelineNode() = default;
        GraphicsPipelineNode(const GraphicsPipelineNode &) = delete;
        GraphicsPipelineNode &operator=(const GraphicsPipelineNode &) = delete;
        GraphicsPipelineNode(GraphicsPipelineNode &&) = delete;
        GraphicsPipelineNode &operator=(GraphicsPipelineNode &&) = delete;


        const std::vector<GraphicsPipelineNode *> &getDepends() const { return this->depends; }

        const std::vector<GraphicsPipelineNode *> &getNext() const { return this->next; }

        const RenderPassAttachmentReference &getAttachmentReference() const { return this->attachmentReference; }

        GraphicsPipelineConfigurator &getPipelineConfigurator() const { return *this->configurator; }

        DescIndexPicker genPicker() const { return this->descIndexPickerFunc(); }

        uint32_t getSubpass() const { return this->subpass; }

        void synchronizeNext() const;
    };
} // namespace merutilm::vkh
