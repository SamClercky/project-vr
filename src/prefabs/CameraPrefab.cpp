#include "CameraPrefab.h"
#include "components/Camera.h"
#include "components/Position.h"
#include <entt/entt.hpp>

namespace prefabs {

    void cameraPrefab(entt::registry &registry) {
        registry.ctx().insert_or_assign(components::Camera{800.0f, 600.0f});
    }

}// namespace prefabs
