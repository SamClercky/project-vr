#ifndef OPENGL_PREFAB_DIRLIGHT_H
#define OPENGL_PREFAB_DIRLIGHT_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace prefabs {

	void dirLightPrefab(entt::registry &registry,
                        glm::vec3 direction,
                        glm::vec3 position, // needed for shadows
                        glm::vec3 ambient,
                        glm::vec3 diffuse,
                        glm::vec3 specular);

    void dirLightPrefab(entt::registry &registry); //test
}

#endif// !OPENGL_PREFAB_DIRLIGHT_H
