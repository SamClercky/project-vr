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
  glm::vec3 lookDirection;
  glm::vec3 worldUp;
  glm::vec3 position;

  Camera(float width, float height)
      : lookDirection(glm::vec3{1.f, 0.f, 0.f}),
        worldUp(glm::vec3{0.f, 1.f, 0.f}),
        position(glm::vec3{0.f,0.f,0.f}) {
    update_perspective(width, height);
  }

  void update_perspective(float width, float height) {
    perspective =
        glm::perspective(glm::radians(45.0f), width / height, .1f, 100.f);
  }
};

} // namespace components

#endif // OPENGL_TEST2_CAMERA_H
