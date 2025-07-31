//
// Created by Merutilm on 2025-07-15.
//

#include "Uniform.hpp"

namespace merutilm::vkh {
    Uniform::Uniform(const Core &core, std::unique_ptr<ShaderObjectManager> &&manager) : BufferObject(core, std::move(manager), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT){
        Uniform::attach();
    }

    Uniform::~Uniform() {
        Uniform::detach();
    }

    void Uniform::attach() {
        //nothing to do
    }

    void Uniform::detach() {
        //nothing to do
    }
}
