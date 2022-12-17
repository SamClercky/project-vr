#include "DirLightPrefab.h"

#include "components/DirLight.h"

#include <iostream>

namespace {

    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);

}

void prefabs::dirLightPrefab(entt::registry& registry,
    glm::vec3 direction,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular) {
    auto entity = registry.create();
    registry.emplace<components::DirLight>(entity, direction, ambient, diffuse, specular);
}

void prefabs::dirLightPrefab(entt::registry &registry) {
    prefabs::dirLightPrefab(
        registry,
            direction,
            ambient,
            diffuse,
            specular);
}

