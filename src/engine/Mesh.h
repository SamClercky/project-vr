#ifndef OPENGL_TEST2_MESH_H
#define OPENGL_TEST2_MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
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

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;

  explicit Vertex(glm::vec3 position)
      : position(position),
        normal(glm::vec3{}),
        texCoord(glm::vec2{}) {}

  explicit Vertex(float x, float y, float z)
      : position(glm::vec3{x, y, z}),
        normal(glm::vec3{0.f}),
        texCoord(glm::vec2{0.f}) {}

  explicit Vertex(float x, float y, float z, float tx, float ty)
      : position(glm::vec3{x, y, z}),
        normal(glm::vec3{0.f}),
        texCoord(glm::vec2{tx, ty}) {}
};

class Mesh {
public:
  explicit Mesh(std::vector<Vertex> &&vertexData);
  Mesh(std::vector<Vertex> &&vertexData, std::vector<uint32_t> &&eboData);

  Mesh(Mesh &&other) = default;

  [[nodiscard]] BoundedMeshGuard bind() const noexcept {
    GLsizei numberOfTriangles;
    bool hasEBO = !eboData.empty();
    if (hasEBO)
      numberOfTriangles = (GLsizei)eboData.size();
    else
      numberOfTriangles = (GLsizei)vertexData.size();

    return BoundedMeshGuard{ID, hasEBO, numberOfTriangles};
  }

private:
  GLuint ID{};
  std::vector<Vertex> vertexData;
  std::vector<uint32_t> eboData;
};

}

#endif // OPENGL_TEST2_MESH_H
