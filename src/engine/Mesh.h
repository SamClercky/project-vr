#ifndef OPENGL_TEST2_MESH_H
#define OPENGL_TEST2_MESH_H

#include "glad/glad.h"
#include <vector>

namespace engine {

struct BoundedMeshGuard {
  GLuint ID;
  bool containsEBO;
  GLsizei numberOfTriangles;

  explicit BoundedMeshGuard(GLuint ID, bool containsEBO,
                            GLsizei numberOfTriangles)
      : ID(ID), containsEBO(containsEBO), numberOfTriangles(numberOfTriangles) {
    glBindVertexArray(ID);
  }
  ~BoundedMeshGuard() { glBindVertexArray(0); }

  void draw() const;
};

class Mesh {
public:
  explicit Mesh(std::vector<float> &&vertexData, bool containsTexture = true);
  Mesh(std::vector<float> &&vertexData, std::vector<float> &&eboData,
       bool containsTexture = true);

  Mesh(Mesh &&other) = default;

  [[nodiscard]] BoundedMeshGuard bind() const noexcept {
    GLsizei numberOfTriangles;
    bool hasEBO = !eboData.empty();
    if (hasEBO)
      numberOfTriangles = (GLsizei)eboData.size() / 3;
    else
      numberOfTriangles = (GLsizei)vertexData.size() / 3;

    return BoundedMeshGuard{ID, hasEBO, numberOfTriangles};
  }

private:
  GLuint ID{};
  std::vector<float> vertexData;
  std::vector<float> eboData;
};

}

#endif // OPENGL_TEST2_MESH_H
