#ifndef OPENGL_TEST2_MESH_H
#define OPENGL_TEST2_MESH_H

#include "Texture2D.h"
#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine {

    struct BoundedMeshGuard {
        GLuint ID;
        bool containsEBO;
        GLsizei numberOfTriangles;
        std::vector<std::shared_ptr<Texture2D>> const &textures;

        explicit BoundedMeshGuard(GLuint ID, bool containsEBO,
                                  GLsizei numberOfTriangles,
                                  std::vector<std::shared_ptr<Texture2D>> const &textures)
            : ID(ID), containsEBO(containsEBO), numberOfTriangles(numberOfTriangles),
              textures(textures) {
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

        explicit Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord)
            : position(position),
              normal(normal),
              texCoord(texCoord) {}

        explicit Vertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty)
            : position(glm::vec3{x, y, z}),
              normal(glm::vec3{nx, ny, nz}),
              texCoord(glm::vec2{tx, ty}) {}

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
        explicit Mesh(std::vector<Vertex> &&vertexData, std::shared_ptr<Shader> &shader);
        Mesh(std::vector<Vertex> &&vertexData,
             std::vector<uint32_t> &&eboData,
             std::vector<std::shared_ptr<Texture2D>> &&textures,
             std::shared_ptr<Shader> &shader);

        Mesh(Mesh const &other) = default;
        Mesh(Mesh &&other) = default;

        [[nodiscard]] BoundedMeshGuard bind() const noexcept {
            GLsizei numberOfTriangles;
            bool hasEBO = !eboData.empty();
            if (hasEBO)
                numberOfTriangles = (GLsizei) eboData.size();
            else
                numberOfTriangles = (GLsizei) vertexData.size();

            return BoundedMeshGuard{ID, hasEBO, numberOfTriangles, textures};
        }

    private:
        GLuint ID{};
        std::vector<Vertex> vertexData;
        std::vector<uint32_t> eboData;
        std::vector<std::shared_ptr<Texture2D>> textures;
    };

}// namespace engine

#endif// OPENGL_TEST2_MESH_H
