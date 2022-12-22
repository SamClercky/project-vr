#ifndef PROJECT_VR_THROWABLEBULLETSHOOTERSYSTEM_H
#define PROJECT_VR_THROWABLEBULLETSHOOTERSYSTEM_H

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "engine/Window.h"
#include <entt/entt.hpp>

namespace systems {
    void throwableBulletSystem(entt::registry &registry, engine::Window &window, std::unique_ptr<btDiscreteDynamicsWorld> &world);
}

#endif//PROJECT_VR_THROWABLEBULLETSHOOTERSYSTEM_H
