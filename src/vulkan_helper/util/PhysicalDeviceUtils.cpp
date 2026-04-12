//
// Created by Merutilm on 2026-02-03.
//

#include <vulkan_helper/util/PhysicalDeviceUtils.hpp>

#include "vulkan_helper/base/allocator.hpp"
namespace merutilm::mvk {
    bool PhysicalDeviceUtils::isDeviceSuitable(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physicalDevice, &features);

        const auto indices = findQueueFamilies(physicalDevice, surface);
        return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
               features.geometryShader &&
               indices.isComplete() &&
               checkDeviceExtensionSupport(physicalDevice);
    }

    QueueFamilyIndices PhysicalDeviceUtils::findQueueFamilies(const VkPhysicalDevice physicalDevice,
        const VkSurfaceKHR surface) {
        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            if (const auto &queueFamily = queueFamilies[i]; (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
                indices.graphicsAndComputeFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }
        }
        return indices;
    }

    bool PhysicalDeviceUtils::checkDeviceExtensionSupport(const VkPhysicalDevice physicalDevice) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());
        auto required = std::unordered_set<std::string, StringHasher, std::equal_to<>>(
                PHYSICAL_DEVICE_EXTENSIONS.begin(), PHYSICAL_DEVICE_EXTENSIONS.end());
        for (const auto &[extensionName, specVersion]: availableExtensions) {
            required.erase(extensionName);
        }
        return required.empty();
    }

    VkSurfaceKHR PhysicalDeviceUtils::createDummySurface(VkInstance instance) {
        VkSurfaceKHR result = VK_NULL_HANDLE;
#ifdef _WIN32
        static constexpr auto dummyClassName = L"VKH_DUMMY";
        uint32_t physicalDeviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
        const WNDCLASSEX wcex = {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_OWNDC,
            .lpfnWndProc = DefWindowProc,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = GetModuleHandle(nullptr),
            .hIcon = nullptr,
            .hCursor = nullptr,
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = dummyClassName,
            .hIconSm = nullptr
        };
        RegisterClassEx(&wcex);
        const HWND dummyWindow = CreateWindowEx(0, dummyClassName, L"", 0, 0, 0, 100, 100, nullptr, nullptr, nullptr, nullptr);


        const VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .hinstance = nullptr,
            .hwnd = dummyWindow,
        };

        allocator::invoke(vkCreateWin32SurfaceKHR, instance, &surfaceCreateInfo, nullptr, &result);

#elif __APPLE__

#elif __linux__

#else

#endif

        return result;
    }
} // namespace merutilm::mvk
