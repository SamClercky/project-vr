#ifndef OPENGL_PREFAB_POINTLIGHT_H
#define OPENGL_PREFAB_POINTLIGHT_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace prefabs {

	void pointLightPrefab(entt::registry &registry,
                          glm::vec3 position,
                          glm::vec3 ambient,
                          glm::vec3 diffuse,
                          glm::vec3 specular,
                          float constant,
                          float linear,
                          float quadratic);

    void pointLightPrefab(entt::registry &registry);
}


#endif// !OPENGL_PREFAB_POINTLIGHT_H
