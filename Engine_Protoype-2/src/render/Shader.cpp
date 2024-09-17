#include "Shader.h"

Shader::Shader(const char* VERTEX_PATH, const char* FRAGMENT_PATH, const char* GEOMETRY_PATH, const char* TESS_CONTROL_SHADER_PATH, const char* TESS_EVALUATION_SHADER_PATH)
{
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(VERTEX_PATH);
		fShaderFile.open(FRAGMENT_PATH);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
		vertex_Code = vShaderStream.str();
		fragment_Code = fShaderStream.str();

		if (GEOMETRY_PATH != nullptr)
		{
			gShaderFile.open(GEOMETRY_PATH);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometry_Code = gShaderStream.str();
		}
		if (TESS_CONTROL_SHADER_PATH != nullptr)
		{
			tcShaderFile.open(TESS_CONTROL_SHADER_PATH);
			std::stringstream tcShaderStream;
			tcShaderStream << tcShaderFile.rdbuf();
			tcShaderFile.close();
			tessalation_control_Code = tcShaderStream.str();
		}
		if (TESS_EVALUATION_SHADER_PATH != nullptr)
		{
			teShaderFile.open(TESS_EVALUATION_SHADER_PATH);
			std::stringstream teShaderStream;
			teShaderStream << teShaderFile.rdbuf();
			teShaderFile.close();
			tessalation_evaluation_Code = teShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertex_Code.c_str();
	const char* fShaderCode = fragment_Code.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckShaderError(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckShaderError(fragment, "FRAGMENT");

	unsigned int geometry{};
	if (GEOMETRY_PATH != nullptr)
	{
		const char* gShaderCode = geometry_Code.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		CheckShaderError(geometry, "GEOMETRY");
	}
	unsigned int tessalation_control{};
	if (TESS_CONTROL_SHADER_PATH != nullptr)
	{
		const char* tcShaderCode = tessalation_control_Code.c_str();
		tessalation_control = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tessalation_control, 1, &tcShaderCode, NULL);
		glCompileShader(tessalation_control);
		CheckShaderError(tessalation_control, "TESSELATION C");
	}
	unsigned int tesselation_evalution{};
	if (TESS_EVALUATION_SHADER_PATH != nullptr)
	{
		const char* teShaderCode = tessalation_evaluation_Code.c_str();
		tesselation_evalution = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tesselation_evalution, 1, &teShaderCode, NULL);
		glCompileShader(tesselation_evalution);
		CheckShaderError(tesselation_evalution, "TESSELATION E");
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	if (GEOMETRY_PATH != nullptr)
	{
		glAttachShader(ID, geometry);
	}

	if (TESS_CONTROL_SHADER_PATH != nullptr)
	{
		glAttachShader(ID, tessalation_control);
	}

	if (TESS_EVALUATION_SHADER_PATH != nullptr)
	{
		glAttachShader(ID, tesselation_evalution);
	}

	glLinkProgram(ID);
	CheckShaderError(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
	glDeleteShader(tessalation_control);
	glDeleteShader(tesselation_evalution);

}

void Shader::use()
{
	glUseProgram(ID);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::CheckShaderError(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
