#ifndef OPENGL_LIGHT_DIR_H
#define OPENGL_LIGHT_DIR_H

#include <glm/glm.hpp>

namespace components {

	struct DirLight {
        glm::vec3 direction;
		
		glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

		explicit DirLight(glm::vec3 direction,
                          glm::vec3 ambient,
                          glm::vec3 diffuse,
                          glm::vec3 specular)
            : direction(direction),
              ambient(ambient),
              diffuse(diffuse),
              specular(specular){};

	};

}


#endif// !OPENGL_LIGHT_DIR_H
