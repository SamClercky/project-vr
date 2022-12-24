#include "SpotLightPrefab.h"

#include "components/Light.h"

#include <iostream>

namespace prefabs {

    glm::vec3 position = glm::vec3(3.0f, 3.0f, 3.0f);
    glm::vec3 direction = glm::vec3(-0.4f, -0.3f, -0.3f);
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(15.0f));

}

void prefabs::spotLightPrefab(entt::registry& registry,
    glm::vec3 position,
    glm::vec3 direction,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float constant,
    float linear,
    float quadratic,
    float cutOff,
    float outerCutOff) {
    auto entity = registry.create();
    registry.emplace<components::Light>(entity, position, direction,
        ambient, diffuse, specular, constant, linear, quadratic,
        cutOff, outerCutOff);
}

void prefabs::spotLightPrefab(entt::registry &registry) {
    prefabs::spotLightPrefab(
            registry,
            position,
            direction,
            ambient,
            diffuse,
            specular,
            constant,
            linear,
            quadratic,
            cutOff,
            outerCutOff);
}

