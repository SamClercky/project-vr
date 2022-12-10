#include "Mesh.h"

using namespace engine;

Mesh::Mesh(std::vector<Vertex> &&vertexData, std::shared_ptr<Shader> &shader)
    : Mesh(std::move(vertexData),
           std::move(std::vector<uint32_t>{}),
           std::move(std::vector<std::shared_ptr<Texture2D>>{}),
           shader) {}

Mesh::Mesh(std::vector<Vertex> &&vertexData,
           std::vector<uint32_t> &&eboData,
           std::vector<std::shared_ptr<Texture2D>> &&textures,
           std::shared_ptr<Shader> &shader)
    : vertexData(vertexData),
      eboData(eboData),
      textures(textures) {
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

    auto position_attr = glGetAttribLocation(shader->ID, "position");
    if (position_attr != -1) {
        glEnableVertexAttribArray(position_attr);
        glVertexAttribPointer(position_attr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
    }
    auto normal_attr = glGetAttribLocation(shader->ID, "normal");
    if (normal_attr != -1) {
        glEnableVertexAttribArray(normal_attr);
        glVertexAttribPointer(normal_attr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, normal));
    }
    auto textureCoord_attr = glGetAttribLocation(shader->ID, "texCoord");
    if (textureCoord_attr != -1) {
        glEnableVertexAttribArray(textureCoord_attr);
        glVertexAttribPointer(textureCoord_attr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, texCoord));
    }

    if (!eboData.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * eboData.size(),
                     eboData.data(), GL_STATIC_DRAW);
    }

    // unbind buffers
    glBindVertexArray(0);
}
void BoundedMeshGuard::draw() const {
    BoundedTexture2DGuard guards[16];
    for (uint8_t i = 0; i < textures.size() && i < 16; i++) {
        guards[i] = textures[i]->bind(i);
    }

    if (containsEBO)
        glDrawElements(GL_TRIANGLES, numberOfTriangles, GL_UNSIGNED_INT,
                       nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, numberOfTriangles);
}
