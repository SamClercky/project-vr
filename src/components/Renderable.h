#ifndef OPENGL_TEST2_RENDERABLE_H
#define OPENGL_TEST2_RENDERABLE_H

#include "engine/Model.h"
#include "engine/Renderer.h"
#include <vector>

namespace components {

    struct Renderable {
        std::vector<engine::Model> parts;
        std::shared_ptr<engine::Shader> shaderRef;

        explicit Renderable(std::vector<engine::Model> &&parts,
                            std::shared_ptr<engine::Shader> &shader) : parts(parts),
                                                                       shaderRef(shader) {}
        explicit Renderable(engine::Model &&part)
            : parts(std::vector<engine::Model>{part}) {}
    };

}// namespace components

#endif// OPENGL_TEST2_RENDERABLE_H
