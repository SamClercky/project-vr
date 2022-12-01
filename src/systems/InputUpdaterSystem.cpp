#include "InputUpdaterSystem.h"
#include "components/Camera.h"
#include "components/DeltaTime.h"
#include "components/Position.h"

void systems::inputUpdaterSystem(entt::registry &registry, engine::Window &window) {
  glm::vec3 delta_pos{0.f};
  if (window.is_key_pressed(engine::Window::ButtonDirections::Up)) {
    delta_pos.z += 1.0;
  }
  if (window.is_key_pressed(engine::Window::ButtonDirections::Down)) {
    delta_pos.z += -1.0;
  }
  if (window.is_key_pressed(engine::Window::ButtonDirections::Right)) {
    delta_pos.x += -1.0;
  }
  if (window.is_key_pressed(engine::Window::ButtonDirections::Left)) {
    delta_pos.x += +1.0;
  }
  glm::vec2 mouse_position;
  window.get_cursor_position(mouse_position);

  auto view = registry.view<components::Camera>();
  auto &dt = registry.ctx().get<components::DeltaTime>();
  for (const auto &entity : view) {
    registry.patch<components::Camera>(entity, [&](components::Camera &cam) {
      auto z = cam.lookDirection;
      auto x = glm::normalize(glm::cross(cam.lookDirection, cam.worldUp));
      auto y = glm::cross(z, x);
      glm::mat3 toCamDirectionTrans{x,y,z};

      auto rotated_delta_pos = toCamDirectionTrans * delta_pos;
      cam.position += rotated_delta_pos*((float)dt.dt/1000);

      glm::mat4 lookRotation{1.f};
      lookRotation = glm::rotate(lookRotation, -mouse_position.x*((float)dt.dt/1000), glm::vec3{0.f, 1.f, 0.f});
      lookRotation = glm::rotate(lookRotation, -mouse_position.y*((float)dt.dt/1000), glm::vec3{0.f, 0.f, 1.f});
      cam.lookDirection = glm::mat3{lookRotation} * cam.lookDirection;
    });
  }
}
