#ifndef PROJECT_VR_DRAWSYSTEM_H
#define PROJECT_VR_DRAWSYSTEM_H

#include "engine/Renderer.h"
#include <entt/entt.hpp>

namespace systems {

    void drawSystem(engine::Renderer::RenderGuard &renderer, entt::registry &registry);

}// namespace systems

#endif// PROJECT_VR_DRAWSYSTEM_H
