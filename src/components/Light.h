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
                       glm::vec3 ambient,
                       glm::vec3 diffuse,
                       glm::vec3 specular,
                       float constant,
                       float linear,
                       float quadratic)
            : light(engine::LightObject{
                      .position = position,
                      .ambient = ambient,
                      .diffuse = diffuse,
                      .specular = specular,
                      .constant = constant,
                      .linear = linear,
                      .quadratic = quadratic,
              }) {}

        explicit Light(glm::vec3 position,
                       glm::vec3 direction,
                       glm::vec3 ambient,
                       glm::vec3 diffuse,
                       glm::vec3 specular,
                       float constant,
                       float linear,
                       float quadratic,
                       float cutOff,
                       float outerCutOff)
            : light(engine::LightObject{
            .position = position,
            .direction = direction,
            .ambient = ambient,
            .diffuse = diffuse,
            .specular = specular,
            .constant = constant,
            .linear = linear,
            .quadratic = quadratic,
            .cutOff = cutOff,
            .outerCutOff = outerCutOff}) {}

	};

}


#endif// !OPENGL_LIGHT_DIR_H
