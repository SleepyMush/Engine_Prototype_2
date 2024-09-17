#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader 
{
public:
	unsigned int ID;
	Shader(const char* VERTEX_PATH,
		const char* FRAGMENT_PATH,
		const char* GEOMETRY_PATH = nullptr,
		const char* TESS_CONTROL_SHADER_PATH = nullptr,
		const char* TESS_EVALUATION_SHADER_PATH = nullptr);

	void use();
	~Shader();

	void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
	std::string vertex_Code;
	std::string fragment_Code;
	std::string geometry_Code;
	std::string tessalation_control_Code;
	std::string tessalation_evaluation_Code;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	std::ifstream tcShaderFile;
	std::ifstream teShaderFile;

	void CheckShaderError(GLuint shader, std::string type);
};