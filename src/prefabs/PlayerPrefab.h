#ifndef PROJECT_VR_PLAYERPREFAB_H
#define PROJECT_VR_PLAYERPREFAB_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

namespace prefabs {
    void playerPrefab(entt::registry &registry, std::unique_ptr<btDiscreteDynamicsWorld> &world);
}

#endif//PROJECT_VR_PLAYERPREFAB_H
