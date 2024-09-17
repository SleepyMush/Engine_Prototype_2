#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 Up, glm::vec3 front, float yaw, float pitch)
{
	Position = position;
	WorldUp = Up;
	Yaw = yaw;
	Pitch = pitch;
	Front = front;

}

glm::mat4 Camera::GetCameraMatrix()
{
	return glm::lookAt(Position, Position + Front, WorldUp);
}


void Camera::CameraUpdate()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	ActualUp = glm::normalize(glm::cross(WorldUp, Front));
	Right = glm::normalize(glm::cross(Front, ActualUp));
}

void Camera::debugcamera() 
{
	printf("Pos:\t\t%f , %f , %f\n", Position.x, Position.y, Position.z);
	printf("Forward:\t%f , %f , %f\n", Front.x, Front.y, Front.z);
	printf("Right:\t\t%f , %f , %f\n", Right.x, Right.y, Right.z);
	printf("Up:\t\t%f , %f , %f\n", WorldUp.x, WorldUp.y, WorldUp.z);
}