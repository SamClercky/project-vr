#ifndef PROJECT_VR_PAINTINGPREFAB_H
#define PROJECT_VR_PAINTINGPREFAB_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace prefabs {
   void screenPrefab(entt::registry &registry, glm::vec3 position, glm::mat3 orientation);
}

#endif//PROJECT_VR_PAINTINGPREFAB_H
