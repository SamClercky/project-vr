#include "DestroyFallOutOfWorldSystem.h"

#include "components/Position.h"
#include "components/ShortLivedObject.h"

void systems::destroyFallenOutOfWorldSystem(entt::registry &registry) {
    const auto &viewPosition = registry.view<components::Position>();
    for (const auto &entity: viewPosition) {
        const auto &position = viewPosition.get<components::Position>(entity);
        const glm::vec3 pos = position.get_translation();

        if (pos.y < -100.f)
            registry.destroy(entity);
    }

    const auto &dt = registry.ctx().get<components::DeltaTime>();
    const auto &viewShortLived = registry.view<components::ShortLivedObject>();
    for (const auto &entity: viewShortLived) {
        auto &ttl = viewShortLived.get<components::ShortLivedObject>(entity);

        if (ttl.time_to_live < dt.dt)
            registry.destroy(entity);
        else
            ttl -= dt;
    }
}
