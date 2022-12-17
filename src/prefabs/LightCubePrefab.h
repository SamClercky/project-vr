#ifndef OPENGL_PREFAB_LIGHTCUBE_H
#define OPENGL_PREFAB_LIGHTCUBE_H

#include "engine/Model.h"
#include <entt/entt.hpp>

namespace prefabs {

    void lightCubePrefab(std::shared_ptr<engine::Model> &asset,
                         std::shared_ptr<engine::Shader> &shader,
                         entt::registry &registry,
                         glm::vec3 position);

    void lightCubePrefabLoader(std::shared_ptr<engine::Model> &outModel,
                               std::shared_ptr<engine::Shader> &outShader);

}// namespace prefabs


#endif// !OPENGL_PREFAB_LIGHTCUBE_H