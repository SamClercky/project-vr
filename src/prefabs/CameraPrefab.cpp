#include "CameraPrefab.h"
#include <entt/entt.hpp>
#include "components/Camera.h"
#include "components/Position.h"

namespace prefabs {

void cameraPrefab(entt::registry &registry) {
  auto cameraEntity = registry.create();
  registry.emplace<components::Camera>(cameraEntity, 800.0f, 600.0f);
  registry.emplace<components::Position>(cameraEntity,
                                         glm::vec3{0.0, 0.0, -3.0});
}

}

