#ifndef OPENGL_TEST2_RENDERABLE_H
#define OPENGL_TEST2_RENDERABLE_H

#include "engine/Model.h"
#include "engine/Renderer.h"
#include <vector>

namespace components {

    struct Renderable {
        std::vector<std::shared_ptr<engine::Model>> parts;
        std::shared_ptr<engine::Shader> shaderRef;
        bool hasShadow;

        explicit Renderable(std::vector<std::shared_ptr<engine::Model>> &&parts,
                            std::shared_ptr<engine::Shader> &shader)
                : parts(parts), shaderRef(shader), hasShadow(true) {}

        explicit Renderable(std::vector<std::shared_ptr<engine::Model>> &&parts,
                            std::shared_ptr<engine::Shader> &shader, bool hasShadow)
            : parts(parts), shaderRef(shader), hasShadow(hasShadow) {}

        explicit Renderable(std::shared_ptr<engine::Model> &&part, bool hasShadow)
            : parts(std::vector<std::shared_ptr<engine::Model>>{part}), hasShadow(hasShadow) {}
        explicit Renderable(std::shared_ptr<engine::Model> &&part)
                : parts(std::vector<std::shared_ptr<engine::Model>>{part}), hasShadow(true) {}
    };

}// namespace components

#endif// OPENGL_TEST2_RENDERABLE_H
