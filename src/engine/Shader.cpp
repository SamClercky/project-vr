#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>

using namespace engine;

Shader::Shader(const std::filesystem::path &vertex, const std::filesystem::path &fragment)
    : Shader(vertex, std::filesystem::path{}, fragment) {}

bool loadFile(const std::filesystem::path &path, std::string &outCode) {
    if (path.empty()) return true;

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream ss;

        ss << file.rdbuf();
        file.close();

        outCode = ss.str();
    } catch (std::ifstream::failure &e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
        return false;
    }

    return true;
}

bool compileShader(std::string& code, GLenum shaderType, uint32_t &outID) {
    if (code.empty()) return true;

    int success;
    char infoLog[512];
    const char *code_cstr = code.c_str();

    outID = glCreateShader(shaderType);
    glShaderSource(outID, 1, &code_cstr, nullptr);
    glCompileShader(outID);
    glGetShaderiv(outID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(outID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED" << std::endl
                  << infoLog << std::endl;
        std::cerr << "Faulty code:\n" << code << std::endl;
    } else {
        std::cout << "SUCCESS::SHADER::COMPILATION_SUCCESSFUL" << std::endl;
    }

    return success;
}

Shader::Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &geometryPath, const std::filesystem::path &fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    loadFile(vertexPath, vertexCode);
    loadFile(geometryPath, geometryCode);
    loadFile(fragmentPath, fragmentCode);

    unsigned int vertex, fragment, geometry;
    compileShader(vertexCode, GL_VERTEX_SHADER, vertex);
    compileShader(geometryCode, GL_GEOMETRY_SHADER, geometry);
    compileShader(fragmentCode, GL_FRAGMENT_SHADER, fragment);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    if (!geometryCode.empty()) glAttachShader(ID, geometry);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl
                  << infoLog << std::endl;
    } else {
        std::cout << "SUCCESS::SHADER::PROGRAM::LINK_SUCCESS" << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) {
    glUniform1i(getUniformLocation(name), (int) value);
}
void Shader::setBool(const std::string &name, int index, bool value) {
    glUniform1i(getUniformLocation(name, index), (int) value);
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(getUniformLocation(name), value);
}
void Shader::setInt(const std::string &name, int index, int value) {
    glUniform1i(getUniformLocation(name, index), value);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(getUniformLocation(name), value);
}
void Shader::setFloat(const std::string &name, int index, float value) {
    glUniform1f(getUniformLocation(name, index), value);
}

void Shader::setMat4f(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                       glm::value_ptr(mat));
}
void Shader::setMat4f(const std::string &name, int index, const glm::mat4 &mat) {
    glUniformMatrix4fv(getUniformLocation(name, index), 1, GL_FALSE,
                       glm::value_ptr(mat));
}

void Shader::setVec3f(const std::string &name, const glm::vec3 &vec) {
    glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
}
void Shader::setVec3f(const std::string &name, int index, const glm::vec3 &vec) {
    glUniform3f(getUniformLocation(name, index), vec.x, vec.y, vec.z);
}

int Shader::getUniformLocation(const std::string &loc) {
    if (cachedLocations.contains(loc))
        return cachedLocations.at(loc);
    else {
        auto gl_loc = glGetUniformLocation(ID, loc.c_str());
        cachedLocations.insert(std::make_pair(loc, gl_loc));
        return gl_loc;
    }
}
int Shader::getUniformLocation(const std::string &loc, int index) {
    auto key = loc;
    key[index   % key.size()] = (char)(index         % 255);
    key[index+1 % key.size()] = (char)((index >>  8) % 255);
    key[index+2 % key.size()] = (char)((index >> 16) % 255);
    key[index+3 % key.size()] = (char)((index >> 24) % 255);

    if (cachedLocations.contains(key))
        return cachedLocations.at(key);
    else {
        auto actual_loc = std::vformat(loc, std::make_format_args(index));
        auto gl_loc = glGetUniformLocation(ID, actual_loc.c_str());
        cachedLocations.insert(std::make_pair(key, gl_loc));

        return gl_loc;
    }
}
