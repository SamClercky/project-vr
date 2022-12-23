#include "DirLightPrefab.h"

#include "components/Light.h"

namespace prefabs {

    glm::vec3 direction = glm::vec3(-1.f, -1.0f, -1.f);
    glm::vec3 position = glm::vec3(0.f, 10.f, 0.f); // needed for shadows
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);

}// namespace prefabs

void prefabs::dirLightPrefab(entt::registry &registry,
                             glm::vec3 direction,
                             glm::vec3 position, // needed for shadows
                             glm::vec3 ambient,
                             glm::vec3 diffuse,
                             glm::vec3 specular) {
    auto entity = registry.create();
    registry.emplace<components::Light>(entity, direction, position, ambient, diffuse, specular);
}

void prefabs::dirLightPrefab(entt::registry &registry) {
    prefabs::dirLightPrefab(
            registry,
            direction,
            position,
            ambient,
            diffuse,
            specular);
}
