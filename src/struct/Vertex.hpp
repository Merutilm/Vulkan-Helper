//
// Created by Merutilm on 2025-07-15.
//

#pragma once
#include <glm/vec3.hpp>

namespace merutilm::vkh {
    struct Vertex {
        glm::vec3 position = {};
        glm::vec3 color = {};

        static Vertex generate(const glm::vec3 &position, const glm::vec3 &color) {
            return Vertex{glm::vec3{position.x, -position.y, position.z}, color};
        }
    };
}
