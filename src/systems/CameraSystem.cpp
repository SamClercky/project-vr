#include "CameraSystem.h"
#include "components/Camera.h"
#include "components/Position.h"

namespace systems {

void cameraSystem(engine::Renderer::RenderGuard &renderer, entt::registry &registry) {
  auto view = registry.view<components::Camera>();
  for (auto &cameraEntity : view) {
    auto &cam = view.get<components::Camera>(cameraEntity);
    renderer.submit_camera(
        cam.perspective,
        glm::lookAt(cam.position, cam.position + cam.get_look_direction(), cam.worldUp)
    );
  }
}

} // namespace systems