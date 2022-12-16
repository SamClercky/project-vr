#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace engine;

Shader::Shader(std::filesystem::path &&vertex, std::filesystem::path &&fragment)
    : Shader(std::move(vertex), std::filesystem::path{}, std::move(fragment)) {}

bool loadFile(std::filesystem::path &&path, std::string &outCode) {
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

Shader::Shader(std::filesystem::path &&vertexPath, std::filesystem::path &&geometryPath, std::filesystem::path &&fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    loadFile(std::move(vertexPath), vertexCode);
    loadFile(std::move(geometryPath), geometryCode);
    loadFile(std::move(fragmentPath), fragmentCode);

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

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

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4f(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}
