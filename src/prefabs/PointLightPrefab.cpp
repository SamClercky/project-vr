#include "PointLightPrefab.h"

#include "components/Light.h"

namespace prefabs {

	glm::vec3 plPosition = glm::vec3(3.0f, 3.0f, 3.0f);
    glm::vec3 plAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 plDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 plSpecular = glm::vec3(1.0f, 1.0f, 1.0f); 
    float plConstant = 1.0f;
    float plLinear = 0.09f;
    float plQuadratic = 0.032f;
}

void prefabs::pointLightPrefab(entt::registry& registry,
    glm::vec3 position,
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float constant,
    float linear,
    float quadratic) {
    auto entity = registry.create();
    registry.emplace<components::Light>(entity, position, ambient, diffuse, specular,
                                        constant, linear, quadratic);
}

void prefabs::pointLightPrefab(entt::registry& registry) {
    prefabs::pointLightPrefab(
            registry,
            plPosition,
            plAmbient,
            plDiffuse,
            plSpecular,
            plConstant,
            plLinear,
            plQuadratic);
}