
#include <filesystem>
#include <fstream>
#include <iostream>

#include "obj/Application.hpp"

void counter(const std::filesystem::path &path, uint32_t *lines) {
    if (std::filesystem::is_directory(path)) {;
        for (std::filesystem::directory_iterator it(path); it != std::filesystem::directory_iterator(); ++it) {
            auto child = it->path();
            counter(child, lines);
        }
    }else if (path.string().ends_with(".cpp") || path.string().ends_with(".hpp")){

        std::ifstream ifs(path);
        std::string v;
        while (std::getline(ifs, v)) {
            ++*lines;
        }
    }
}


int main() {
    const std::filesystem::path path("../src");
    uint32_t lines = 0;
    counter(path, &lines);
    std::cout << "Lines : " << lines << std::endl;

    using namespace merutilm::vkh;
    const auto app = Application();
    return 0;
}
