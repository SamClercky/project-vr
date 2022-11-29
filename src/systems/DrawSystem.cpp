#include "DrawSystem.h"
#include "components/Renderable.h"
#include "components/Position.h"

namespace systems {

void drawSystem(engine::Renderer::RenderGuard &renderer, entt::registry &registry) {
  const auto view =
      registry.view<components::Renderable, components::Position>();
  for (const auto &entity : view) {
    const auto &renderable = view.get<components::Renderable>(entity);
    const auto &position = view.get<components::Position>(entity);
    for (const auto &part : renderable.parts)
      renderer.submit(part, position.pos);
  }
}

} // namespace systems