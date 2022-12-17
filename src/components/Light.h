#ifndef OPENGL_LIGHT_COMPONENT_H
#define OPENGL_LIGHT_COMPONENT_H

#include <glm/glm.hpp>
#include "engine/Renderer.h"

namespace components {

	struct Light {
        engine::LightObject light;

		explicit Light(glm::vec3 direction,
                       glm::vec3 ambient,
                       glm::vec3 diffuse,
                       glm::vec3 specular)
            : light(engine::LightObject{
                      .direction = direction,
                      .ambient = ambient,
                      .diffuse = diffuse,
                      .specular = specular,
              }) {}
            

        explicit Light(glm::vec3 position,
                       float constant,
                       float linear,
                       float quadratic,
                       glm::vec3 ambient,
                       glm::vec3 diffuse,
                       glm::vec3 specular)
            : light(engine::LightObject{
                      .position = position,
                      .ambient = ambient,
                      .diffuse = diffuse,
                      .specular = specular,
                      .constant = constant,
                      .linear = linear,
                      .quadratic = quadratic,
              }) {}

	};

}


#endif// !OPENGL_LIGHT_DIR_H
