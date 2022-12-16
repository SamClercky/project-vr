#ifndef OPENGL_TEST2_SMOKEPREFAB_H
#define OPENGL_TEST2_SMOKEPREFAB_H

#include "components/Renderable.h"
#include "engine/Model.h"
#include "engine/Renderer.h"
#include <entt/entt.hpp>

namespace prefabs {

    void smokePrefab(std::shared_ptr<engine::Model> &asset,
                    std::shared_ptr<engine::Shader> &shader,
                    entt::registry &registry,
                    glm::vec3 position);
    void smokePrefabLoader(std::shared_ptr<engine::Model> &outModel,
                          std::shared_ptr<engine::Shader> &outShader);

}// namespace prefabs

#endif// OPENGL_TEST2_CUBEPREFAB_H
