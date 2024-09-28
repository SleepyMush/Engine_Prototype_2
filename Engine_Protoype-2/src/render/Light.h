#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "SSBO.h"
#include "GL_shader.h"

class Lights {
private:
	int numLights = 1;
	GLsizeiptr bufferSize = sizeof(Light) * numLights;
	SSBO ssbo;

public:
	struct Light {
		glm::vec3 color;
		float radius_D;
		glm::vec3 position;
		float radius;
	};
	std::vector<Light> lightData;

	Lights() : lightData(numLights) {} 
	~Lights();

	void CreateLights(glm::vec3 color, glm::vec3 position, float radius);
	void UpdateLights(Shader shader);

	// Setter methods
	void setColor(const glm::vec3& color, int index);
	void setPosition(const glm::vec3& position, int index);
	void setRadius(float radius, int index);
};
