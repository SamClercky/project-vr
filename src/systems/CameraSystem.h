#ifndef PROJECT_VR_CAMERASYSTEM_H
#define PROJECT_VR_CAMERASYSTEM_H

#include "engine/Renderer.h"
#include <entt/entt.hpp>

namespace systems {

void cameraSystem(engine::Renderer::RenderGuard &renderer, entt::registry &registry);

} // namespace systems

#endif // PROJECT_VR_CAMERASYSTEM_H
