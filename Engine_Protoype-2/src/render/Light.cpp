#include "Light.h"

void Lights::CreateLights(glm::vec3 color, glm::vec3 position, float radius)
{
	ssbo.SSBObuffer(bufferSize);

	for (int i = 0; i < numLights; ++i) {
		setColor(color, i);
		setPosition(position, i);
		setRadius(radius, i);
	}
}

void Lights::UpdateLights(Shader shader)
{
	ssbo.updateSSBO(bufferSize, lightData.data());

	for (int i = 0; i < numLights; ++i) {
		shader.SetVec3("light[0].position", lightData[i].position);
		shader.SetVec3("light[0].color", lightData[i].color);
		shader.SetFloat("light[0].radius", lightData[i].radius);

		shader.SetVec3("color", lightData[i].color);
	}
}

void Lights::setColor(const glm::vec3& color, int index)
{
	if (index >= 0 && index < numLights) {
		lightData[index].color = color;
	}
}

void Lights::setPosition(const glm::vec3& position, int index)
{
	if (index >= 0 && index < numLights) {
		lightData[index].position = position;
	}
}

void Lights::setRadius(float radius, int index)
{
	if (radius >= 0 && index < numLights) {
		lightData[index].radius = radius;
	}

}

Lights::~Lights()
{
	ssbo.clear();
}

