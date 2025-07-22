//
// Created by Merutilm on 2025-07-13.
//

#pragma once
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "../exception/exception.hpp"
#include "../struct/StringHasher.hpp"
#include "../struct/RenderPassAttachmentConfig.hpp"
#include "../struct/RenderPassAttachmentType.hpp"
#include "../util/BufferUtils.hpp"

namespace merutilm::vkh {
    class RenderPassManager {

        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > subpassNames = {};
        std::unordered_map<std::string, uint32_t, StringHasher, std::equal_to<> > attachmentNames = {};

        std::vector<RenderPassAttachmentConfig> attachments = {};
        std::vector<std::vector<uint32_t> > preserveIndices = {};
        std::vector<std::unordered_map<RenderPassAttachmentType, std::vector<VkAttachmentReference> > >
        attachmentReferences{};
        std::vector<VkSubpassDependency> subpassDependencies = {};
        uint32_t subpassCount = 0;
        uint32_t count = 0;

    public:
        explicit RenderPassManager() = default;

        ~RenderPassManager() = default;

        RenderPassManager(const RenderPassManager &) = delete;

        RenderPassManager &operator=(const RenderPassManager &) = delete;

        RenderPassManager(RenderPassManager &&) = delete;

        RenderPassManager &operator=(RenderPassManager &&) = delete;

        [[nodiscard]] uint32_t getPreserveIndicesCount(const uint32_t subpassIndex) const {
            return static_cast<uint32_t>(preserveIndices[subpassIndex].size());
        };

        [[nodiscard]] uint32_t *getPreserveIndices(const uint32_t subpassIndex) {
            return preserveIndices[subpassIndex].data();
        }

        [[nodiscard]] const std::vector<RenderPassAttachmentConfig> &getAttachmentConfigs() const {
            return attachments;
        }


        [[nodiscard]] uint32_t getSubpassCount() const { return subpassCount; }


        [[nodiscard]] const std::vector<VkAttachmentReference> &getAttachmentReferences(
            const uint32_t subpassIndex, const RenderPassAttachmentType attachmentType) const {
            return attachmentReferences[subpassIndex].at(attachmentType);
        }

        [[nodiscard]] const std::vector<VkSubpassDependency> &getSubpassDependencies() const {
            return subpassDependencies;
        }

        void setPreserved(const std::string &attachmentName);

        void unsetPreserved(const std::string &attachmentName);

        void appendSubpass(const std::string &subpassName);

        void appendAttachment(const std::string &attachmentName, RenderPassAttachmentType attachmentType,
                              const VkAttachmentDescription &attachment,
                              const ImageInitInfo &imageInitInfo = {});

        uint32_t getSubpassIndex(const std::string &subpassName) const;


        void appendDependency(const VkSubpassDependency &subpassDependency);
    };


    inline void RenderPassManager::setPreserved(const std::string &attachmentName) {
        if (!attachmentNames.contains(attachmentName)) {
            throw exception_invalid_args("Invalid attachment name");
        }
        preserveIndices.back().emplace_back(attachmentNames[attachmentName]);
    }


    inline void RenderPassManager::unsetPreserved(const std::string &attachmentName) {
        if (!attachmentNames.contains(attachmentName)) {
            throw exception_invalid_args("Invalid attachment name");
        }

        auto &indices = preserveIndices.back();
        const uint32_t preserveIndex = attachmentNames[attachmentName];
        const auto it = std::ranges::find(indices, preserveIndex);
        if (it == indices.end()) {
            throw exception_invalid_args("given attachment is not preserved");
        }
        indices.erase(it);
    }


    inline void RenderPassManager::appendSubpass(const std::string &subpassName) {
        using enum RenderPassAttachmentType;
        if (subpassNames.contains(subpassName)) {
            throw exception_invalid_args("Subpass already exists");
        }
        attachmentReferences.emplace_back();
        attachmentReferences.back()[INPUT];
        attachmentReferences.back()[COLOR];
        attachmentReferences.back()[RESOLVE];
        attachmentReferences.back()[DEPTH_STENCIL];
        preserveIndices.emplace_back();
        subpassNames[subpassName] = subpassCount;
        ++subpassCount;
    }

    inline void RenderPassManager::appendAttachment(const std::string &attachmentName,
                                                    const RenderPassAttachmentType attachmentType,
                                                    const VkAttachmentDescription &attachment,
                                                    const ImageInitInfo &imageInitInfo) {
        using enum RenderPassAttachmentType;
        if (attachmentNames.contains(attachmentName)) {
            throw exception_invalid_args("Attachment already exists");
        }

        auto &ref = this->attachmentReferences.back()[attachmentType];
        VkAttachmentReference attachmentReference = {};

        switch (attachmentType) {
            case INPUT: {
                attachmentReference = {
                    .attachment = count,
                    .layout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
                };

                break;
            }
            case COLOR:
            case RESOLVE: {
                attachmentReference = {
                    .attachment = count,
                    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                };
                break;
            }
            case DEPTH_STENCIL: {
                if (!ref.empty()) {
                    throw exception_invalid_args("Depth Stencil attachment has already been set");
                }
                attachmentReference = {
                    .attachment = count,
                    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                };
                break;
            }
        }


        this->attachments.emplace_back(attachmentType, attachment, imageInitInfo);

        ref.emplace_back(attachmentReference);
        attachmentNames[attachmentName] = count;
        ++count;
    }

    inline uint32_t RenderPassManager::getSubpassIndex(const std::string &subpassName) const {
        if (!subpassNames.contains(subpassName)) {
            throw exception_invalid_args("Invalid subpass name");
        }
        return subpassNames.at(subpassName);
    }

    inline void RenderPassManager::appendDependency(const VkSubpassDependency &subpassDependency) {
        subpassDependencies.push_back(subpassDependency);
    }
}
