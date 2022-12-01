#ifndef OPENGL_TEST2_POSITION_H
#define OPENGL_TEST2_POSITION_H

#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>

namespace components {

struct Position {
  glm::mat4 pos{1.0};

  explicit Position(glm::mat4 M) : pos(M) {}
  explicit Position(glm::vec3 translation)
      : Position(translation, glm::mat3{1.0}) {}
  explicit Position(glm::mat3 rotation) : Position(glm::vec3{0.0}, rotation) {}
  Position(glm::vec3 translation, glm::mat3 rotation) {
    pos = glm::mat4{rotation};
    pos = glm::translate(pos, translation);
  }

  glm::mat3 get_rotation() {
    return glm::mat3{
        glm::vec3{pos[0]},
        glm::vec3{pos[1]},
        glm::vec3{pos[2]},
    };
  }

  glm::vec3 get_translation() {
    return glm::vec3{pos[3]};
  }
};

} // namespace components

#endif // OPENGL_TEST2_POSITION_H
