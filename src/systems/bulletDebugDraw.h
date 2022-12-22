#ifndef PROJECT_VR_BULLETDEBUGDRAW_H
#define PROJECT_VR_BULLETDEBUGDRAW_H

#include <entt/entt.hpp>
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include <memory>
#include "engine/Renderer.h"

namespace systems {
    void bulletDebugDrawSystem(entt::registry &registry, engine::Renderer::RenderGuard &guard, std::unique_ptr<btDiscreteDynamicsWorld> &world);
}

#endif//PROJECT_VR_BULLETDEBUGDRAW_H
