#ifndef PROJECT_VR_BULLETSYSTEM_H
#define PROJECT_VR_BULLETSYSTEM_H

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include <entt/entt.hpp>

namespace systems {
    void bulletSystem(entt::registry &registry, std::unique_ptr<btDiscreteDynamicsWorld> &dynamicWorld);
}

#endif//PROJECT_VR_BULLETSYSTEM_H
