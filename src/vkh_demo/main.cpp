
#include <filesystem>
#include <fstream>
#include <iostream>

#include "DemoApplication.hpp"


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

    // constexpr siv::PerlinNoise::seed_type seed = 123456u;
    //
    // const siv::PerlinNoise perlin{ seed };
    //
    // for (int y = 0; y < 22; ++y)
    // {
    //     for (int x = 0; x < 8; ++x)
    //     {
    //         const double noise = perlin.normalizedOctave2D_01((x * 0.01), (y * 0.01), 4);
    //
    //         std::cout << noise << '\t';
    //     }
    //
    //     std::cout << '\n';
    // }


    const std::filesystem::path path("../src");
    const std::filesystem::path path2("../include");
    uint32_t lines = 0;
    counter(path, &lines);
    counter(path2, &lines);
    std::cout << "Lines : " << lines << std::endl;
    const auto application = merutilm::vkh_demo::DemoApplication();
    application.start();

    return 0;
}
