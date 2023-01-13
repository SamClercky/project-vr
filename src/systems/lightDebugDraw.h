#ifndef PROJECT_VR_LIGHTDEBUGDRAW_H
#define PROJECT_VR_LIGHTDEBUGDRAW_H

#include "engine/Renderer.h"
#include "entt/entity/registry.hpp"

namespace systems {
    void lightDebugDrawSystem(entt::registry &registry, engine::Renderer::RenderGuard &guard);
}

#endif//PROJECT_VR_LIGHTDEBUGDRAW_H
