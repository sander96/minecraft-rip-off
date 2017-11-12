#include "Camera.h"
#include <iostream>
#include "utilities/Settings.h"

Camera::Camera()
{
	Settings settings;
	float aspectRatio = 1.6f;
	perspective = glm::perspective(glm::radians(settings.getFov()), aspectRatio, 0.1f, 100.0f);

	position = glm::vec3(0.0, 0.0, 10.0);
	lookAt = glm::vec3(0.0, 0.0, -1.0);
	view = glm::lookAt(
		position,
		lookAt + position,
		glm::vec3(0.0, 1.0, 0.0)
	);
}

void Camera::move(glm::vec3 movement)
{
	glm::vec3 forwardVector = lookAt;
	glm::normalize(forwardVector);

	//moving forward
	position += movement[2] * forwardVector;

	//moving left
	glm::vec3 leftVector = glm::cross(glm::vec3(0.0, 1.0, 0.0), forwardVector);
	position += movement[0] * leftVector;

	view = glm::lookAt(
		position,
		lookAt + position,
		glm::vec3(0.0, 1.0, 0.0)
	);
}

void Camera::rotate(glm::vec2 rotation)
{
	//OTI ARVAMUS, kaameral on see viga, et on world axisi järgi rotate, kuidagi localiks?
	lookAt = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[0]), glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(lookAt, 0.0);
	lookAt = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[1]), glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(lookAt, 0.0);
	view = glm::lookAt(
		position,
		lookAt + position,
		glm::vec3(0.0, 1.0, 0.0)
	);
}

glm::mat4 Camera::getPerspective()
{
	return perspective;
}

glm::mat4 Camera::getView()
{
	return view;
}
