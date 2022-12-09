#include "Mesh.h"

using namespace engine;

Mesh::Mesh(std::vector<Vertex> &&vertexData)
    : Mesh(std::move(vertexData), std::move(std::vector<uint32_t>{})) {}

Mesh::Mesh(std::vector<Vertex> &&vertexData, std::vector<uint32_t> &&eboData)
    : vertexData(vertexData), eboData(eboData) {
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    ID = VAO;// store for later use

    glGenBuffers(1, &VBO);
    if (!eboData.empty())
        glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexData.size(),
                 vertexData.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, texCoord));

    if (!eboData.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * eboData.size(),
                     eboData.data(), GL_STATIC_DRAW);
    }

    // unbind buffers
    if (!eboData.empty())
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void BoundedMeshGuard::draw() const {
    if (containsEBO)
        glDrawElements(GL_TRIANGLES, numberOfTriangles, GL_UNSIGNED_INT,
                       nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, numberOfTriangles);
}
