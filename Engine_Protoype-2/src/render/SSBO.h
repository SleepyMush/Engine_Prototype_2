#pragma once
#include <glad/glad.h>

class SSBO {
private:
	GLuint ssbo;
public:
	void SSBObuffer(GLsizeiptr size);
	void updateSSBO(GLsizeiptr size, void* data);
	void clear();
};