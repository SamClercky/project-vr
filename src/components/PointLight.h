#ifndef OPENGL_LIGHT_POINT_H
#define OPENGL_LIGHT_POINT_H

namespace components {

    struct PointLight {
        glm::vec3 position;

        float constant;
        float linear;
        float quadratic;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };


}// namespace components

#endif// !OPENGL_LIGHT_POINT_H
