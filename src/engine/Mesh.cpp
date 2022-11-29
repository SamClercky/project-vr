#include "Mesh.h"

using namespace engine;

 Mesh::Mesh(std::vector<float> &&vertexData, bool containsTexture)
     : Mesh(std::move(vertexData), std::move(std::vector<float>{}),
            containsTexture) {}

 Mesh::Mesh(std::vector<float> &&vertexData, std::vector<float> &&eboData,
            bool containsTexture)
     : vertexData(vertexData), eboData(eboData) {
   GLuint VAO, VBO, EBO;

   glGenVertexArrays(1, &VAO);
   ID = VAO; // store for later use

   glGenBuffers(1, &VBO);
   if (!eboData.empty())
     glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(),
                vertexData.data(), GL_STATIC_DRAW);

   GLsizei stride = 3 * sizeof(float); // only 3 floats for position
   if (containsTexture)
     stride = 5 * sizeof(float); // add extra 2 floats for texture coords
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
   if (containsTexture) {
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                           (void *)(3 * sizeof(float)));
   }

   if (!eboData.empty()) {
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * eboData.size(),
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
