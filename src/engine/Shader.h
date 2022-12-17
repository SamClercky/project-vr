#ifndef OPENGL_TEST2_SHADER_H
#define OPENGL_TEST2_SHADER_H

#include "glm/glm.hpp"
#include <glad/glad.h>
#include <string>

namespace engine {

    class Shader {
    public:
        unsigned int ID;

        Shader(const char *vertexPath, const char *fragmentPath);
        Shader(Shader &&other) = default;

        void use() const;
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4f(const std::string &name, const glm::mat4 &mat) const;
        void setVec3f(const std::string &name, const glm::vec3 &vec) const;
    };

}// namespace engine

#endif// OPENGL_TEST2_SHADER_H
