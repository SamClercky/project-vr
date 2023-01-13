#ifndef OPENGL_TEST2_SHADER_H
#define OPENGL_TEST2_SHADER_H

#include "glm/glm.hpp"
#include <filesystem>
#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace engine {

    class Shader {
    public:
        unsigned int ID;

        Shader(const std::filesystem::path &vertex, const std::filesystem::path &fragment);
        Shader(const std::filesystem::path &vertex,
               const std::filesystem::path &geometry,
               const std::filesystem::path &fragment);
        Shader(Shader &&other) = default;

        void use() const;
        void setBool(const std::string &name, bool value);
        void setBool(const std::string &name, int index, bool value);
        void setInt(const std::string &name, int value);
        void setInt(const std::string &name, int index, int value);
        void setFloat(const std::string &name, float value);
        void setFloat(const std::string &name, int index, float value);
        void setMat4f(const std::string &name, const glm::mat4 &mat);
        void setMat4f(const std::string &name, int index, const glm::mat4 &mat);
        void setVec3f(const std::string &name, const glm::vec3 &vec);
        void setVec3f(const std::string &name, int index, const glm::vec3 &vec);

    private:
        std::unordered_map<std::string, int> cachedLocations;

        int getUniformLocation(const std::string &loc);
        int getUniformLocation(const std::string &loc, int index);
    };

}// namespace engine

#endif// OPENGL_TEST2_SHADER_H
