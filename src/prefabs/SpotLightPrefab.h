#ifndef OPENGL_PREFAB_SPOTLIGHT_H
#define OPENGL_PREFAB_SPOTLIGHT_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace prefabs {

	void spotLightPrefab(entt::registry &registry,
                        glm::vec3 position,
                        glm::vec3 direction,
                        glm::vec3 ambient,
                        glm::vec3 diffuse,
                        glm::vec3 specular,
                        float constant,
                        float linear,
                        float quadratic,
                        float cutOff,
                        float outerCutOff);

    void spotLightPrefab(entt::registry &registry); //test
}

#endif// !OPENGL_PREFAB_SPOTLIGHT_H
