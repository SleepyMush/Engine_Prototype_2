#include "GL_shader.h"
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::string readTextFromFile(std::string path) {
    std::ifstream file(path);
    std::string str;
    std::string line;
    while (std::getline(file, line)) {
        str += line + "\n";
    }
    return str;
}

int checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
    return success;
}

void Shader::Load(std::string vertexPath, std::string fragmentPath) {

    std::string vertexSource = readTextFromFile("res/shader/" + vertexPath);
    std::string fragmentSource = readTextFromFile("res/shader/" + fragmentPath);

    const char* vShaderCode = vertexSource.c_str();
    const char* fShaderCode = fragmentSource.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    int tempID = glCreateProgram();
    glAttachShader(tempID, vertex);
    glAttachShader(tempID, fragment);
    glLinkProgram(tempID);

    if (checkCompileErrors(tempID, "PROGRAM")) {
        if (m_ID != -1) {
            glDeleteProgram(m_ID);
        }
        m_ID = tempID;
        m_uniformsLocations.clear();
    }
    else {
        std::cout << "shader failed to compile " << vertexPath << " " << fragmentPath << "\n";
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(m_ID);
}

void Shader::SetBool(const std::string& name, bool value) {
    if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
        m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
    glUniform1i(m_uniformsLocations[name], (int)value);
}

void Shader::SetInt(const std::string& name, int value) {
    if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
        m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
    glUniform1i(m_uniformsLocations[name], value);
}

void Shader::SetFloat(const std::string& name, float value) {
    if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
        m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
    glUniform1f(m_uniformsLocations[name], value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) {
    if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
        m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
    glUniformMatrix4fv(m_uniformsLocations[name], 1, GL_FALSE, &value[0][0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
    if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
        m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
    glUniform4fv(m_uniformsLocations[name], 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
        m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
    glUniform3fv(m_uniformsLocations[name], 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
    if (m_uniformsLocations.find(name) == m_uniformsLocations.end())
        m_uniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
    glUniform2fv(m_uniformsLocations[name], 1, &value[0]);
}