#include "RotateSystem.h"
#include "components/DeltaTime.h"
#include "components/Position.h"
#include "components/RotateAnimation.h"

namespace systems {

void rotateSystem(entt::registry &registry) {
  const auto dt = registry.ctx().get<components::DeltaTime>();
  auto view =
      registry.view<components::Position, components::RotateAnimation>();

  for (auto &entity : view) {
    auto position = view.get<components::Position>(entity);
    auto animation = view.get<components::RotateAnimation>(entity);

    registry.patch<components::Position>(entity, [&](auto &pos) {
      pos.pos = glm::rotate(position.pos,
                            ((float)dt.dt) / 1000.f *
                            glm::radians(animation.rotationSpeed),
                            glm::vec3{1., 1., 1.});
    });
  }
}

} // namespace systems