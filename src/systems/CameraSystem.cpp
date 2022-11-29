#include "CameraSystem.h"
#include "components/Camera.h"
#include "components/Position.h"

namespace systems {

void cameraSystem(engine::Renderer::RenderGuard &renderer, entt::registry &registry) {
  auto view = registry.view<components::Camera, components::Position>();
  for (auto &cameraEntity : view) {
    renderer.submit_camera(
        view.get<components::Camera>(cameraEntity).perspective,
        view.get<components::Position>(cameraEntity).pos);
  }
}

} // namespace systems