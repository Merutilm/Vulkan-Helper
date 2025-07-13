//
// Created by Merutilm on 2025-07-09.
//

#pragma once

#include "Instance.hpp"
#include "Surface.hpp"
#include "../struct/QueueFamilyIndices.hpp"
#include "../objhandler/Handler.hpp"

namespace merutilm::vkh {
    class PhysicalDevice final : public Handler {

        const Instance &instance;
        const Surface &surface;

        VkPhysicalDevice physicalDevice = nullptr;
        VkPhysicalDeviceProperties physicalDeviceProperties = {};
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = {};
        VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
        QueueFamilyIndices queueFamilyIndices;

    public:

        inline static const std::vector<const char *> PHYSICAL_DEVICE_EXTENSIONS = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        explicit PhysicalDevice(const Instance &instance, const Surface &surface);

        ~PhysicalDevice() override;

        PhysicalDevice(const PhysicalDevice &) = delete;

        PhysicalDevice &operator=(const PhysicalDevice &) = delete;

        PhysicalDevice(PhysicalDevice &&) = delete;

        PhysicalDevice &operator=(PhysicalDevice &&) = delete;

        [[nodiscard]] VkPhysicalDevice getPhysicalDeviceHandle() const { return physicalDevice; }

        [[nodiscard]] const VkPhysicalDeviceProperties &getPhysicalDeviceProperties() const {
            return physicalDeviceProperties;
        }

        [[nodiscard]] const VkPhysicalDeviceMemoryProperties &getPhysicalDeviceMemoryProperties() const {
            return physicalDeviceMemoryProperties;
        }

        [[nodiscard]] const VkPhysicalDeviceFeatures &getPhysicalDeviceFeatures() const {
            return physicalDeviceFeatures;
        }

        [[nodiscard]] VkSurfaceCapabilitiesKHR populateSurfaceCapabilities() const;

        [[nodiscard]] const QueueFamilyIndices &getQueueFamilyIndices() const { return queueFamilyIndices; }

    private:
        void init() override;

        bool isDeviceSuitable(VkPhysicalDevice physicalDevice) const;

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice) const;

        static bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

        void destroy() override;
    };
}
