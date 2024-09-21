#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 WorldUp;
	glm::vec3 ActualUp;

	glm::vec3 Front;
	glm::vec3 Right;
	float Yaw;
	float Pitch;

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	Camera(glm::vec3 position = glm::vec3(0.0f ,0.0f, 0.0f),
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 front = glm::vec3(0.0f,0.0f,-1.0f),
		float yaw = -90.0f,
		float pitch = 0.0f);

	glm::mat4 GetCameraMatrix();

	float fov = 45.0f;
	float near = 0.1f;
	float far = 100.0f;

	void CameraUpdate();
	void debugcamera();

};
