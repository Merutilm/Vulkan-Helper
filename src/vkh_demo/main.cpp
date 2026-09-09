#include "DemoApplication.hpp"

int main() {
    using namespace merutilm;

    vkh::Application::start<vkh_demo::DemoApplication>({});

    return 0;
}
