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
            
                }) {}
            

        explicit Light(glm::vec3 position,
                       float constant,
                       float linear,
                       float quadratic,
                       glm::vec3 ambient,
                       glm::vec3 diffuse,
                       glm::vec3 specular)
            : light(engine::LightObject{}) {}

	};

}


#endif// !OPENGL_LIGHT_DIR_H
