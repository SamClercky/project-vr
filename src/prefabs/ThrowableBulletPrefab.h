#ifndef PROJECT_VR_THROWABLEBULLETPREFAB_H
#define PROJECT_VR_THROWABLEBULLETPREFAB_H

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "engine/Model.h"
#include "engine/Shader.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace prefabs {
    void loadThrowableBulletPrefab(std::shared_ptr<engine::Model> &outModel, std::shared_ptr<engine::Shader> &outShader);
    void throwableBulletPrefab(entt::registry &registry,
                                        std::unique_ptr<btDiscreteDynamicsWorld> &world,
                                        std::shared_ptr<engine::Model> &model,
                                        std::shared_ptr<engine::Shader> &shader,
                                        glm::vec3 position,
                                        glm::vec3 impulse);
}

#endif//PROJECT_VR_THROWABLEBULLETPREFAB_H
