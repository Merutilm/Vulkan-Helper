//
// Created by Merutilm on 2025-07-18.
//

#pragma once
#include <string>

#include "../context/RenderContext.hpp"
#include "../handle/CoreHandler.hpp"
#include "../obj/Pipeline.hpp"
#include "../obj/ShaderModule.hpp"
#include "../repo/Repositories.hpp"

namespace merutilm::vkh {
    class BasicShaderProgram : public CoreHandler {
        Repositories *repositories;
        const RenderContext &renderContext;
        const std::string subpassName;
        const ShaderModule &vertexShader;
        const ShaderModule &fragmentShader;

        std::unique_ptr<Pipeline> pipeline = nullptr;

    public:
        explicit BasicShaderProgram(const Core &engine, Repositories *repositories,
                                    const RenderContext &renderContext,
                                    const std::string &subpassName,
                                    const std::string &vertName,
                                    const std::string &fragName);

        ~BasicShaderProgram() override;

        BasicShaderProgram(const BasicShaderProgram &) = delete;

        BasicShaderProgram(BasicShaderProgram &&) = delete;

        BasicShaderProgram &operator=(const BasicShaderProgram &) = delete;

        BasicShaderProgram &operator=(BasicShaderProgram &&) = delete;

    private:
        void attach() override;

        void detach() override;
    };
}
