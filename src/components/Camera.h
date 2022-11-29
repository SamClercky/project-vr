//
// Created by decle on 11/29/2022.
//

#ifndef OPENGL_TEST2_CAMERA_H
#define OPENGL_TEST2_CAMERA_H

#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>

namespace components {

struct Camera {
  glm::mat4 perspective;

  Camera(float width, float height) {
    perspective =
        glm::perspective(glm::radians(45.0f), width / height, .1f, 100.f);
  }
};

} // namespace components

#endif // OPENGL_TEST2_CAMERA_H
