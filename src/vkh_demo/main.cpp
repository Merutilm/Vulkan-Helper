#include "DemoApplication.hpp"

int main() {
    using namespace merutilm;

    vkh::Application::start<vkh_demo::DemoApplication>({
        .version = VK_API_VERSION_1_3,
        .instanceExtensions = {},
        .deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        }
    },{});

    return 0;
}
