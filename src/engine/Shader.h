#ifndef OPENGL_TEST2_SHADER_H
#define OPENGL_TEST2_SHADER_H

#include "glm/glm.hpp"
#include <glad/glad.h>
#include <string>
#include <filesystem>

namespace engine {

    class Shader {
    public:
        unsigned int ID;

        Shader(std::filesystem::path &&vertex, std::filesystem::path &&fragment);
        Shader(std::filesystem::path &&vertex,
               std::filesystem::path &&geometry,
               std::filesystem::path &&fragment);
        Shader(Shader &&other) = default;

        void use() const;
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4f(const std::string &name, const glm::mat4 &mat) const;
    };

}// namespace engine

#endif// OPENGL_TEST2_SHADER_H
