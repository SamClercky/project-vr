#include "DestroyFallOutOfWorldSystem.h"

#include "components/Position.h"

void systems::destroyFallenOutOfWorldSystem(entt::registry &registry) {
    const auto &view = registry.view<components::Position>();
    for (const auto entity: view) {
        const auto &position = view.get<components::Position>(entity);
        const glm::vec3 pos = position.get_translation();

        if (pos.y < -100.f)
            registry.destroy(entity);
    }
}
