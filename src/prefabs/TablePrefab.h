#ifndef OPENGL_PREFAB_TABLE_H
#define OPENGL_PREFAB_TABLE_H

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "engine/Model.h"
#include <entt/entt.hpp>

namespace prefabs {

    void lightCubePrefab(std::shared_ptr<engine::Model> &asset,
                         std::shared_ptr<engine::Shader> &shader,
                         entt::registry &registry,
                         glm::vec3 position,
                         std::unique_ptr<btDiscreteDynamicsWorld> &world);

    void lightCubePrefabLoader(std::shared_ptr<engine::Model> &outModel,
                               std::shared_ptr<engine::Shader> &outShader);

}// namespace prefabs


#endif// !OPENGL_PREFAB_TABLE_H