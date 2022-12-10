#include "DrawSystem.h"
#include "components/Position.h"
#include "components/Renderable.h"
#include <vector>

namespace systems {

    void drawSystem(engine::Renderer::RenderGuard &renderer, entt::registry &registry) {
        const auto view =
                registry.view<components::Renderable, components::Position>();
        for (const auto &entity: view) {
            const auto &renderable = view.get<components::Renderable>(entity);
            const auto &position = view.get<components::Position>(entity);
            for (const auto &part: renderable.parts)
                part.draw(renderer, renderable.shaderRef, position.pos, std::vector<glm::mat4>{});
        }
    }

}// namespace systems