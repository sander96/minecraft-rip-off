#include "Camera.h"
#include <iostream>
#include "utilities/Settings.h"

Camera::Camera()
{
	Settings settings;
	float aspectRatio = static_cast<float>(settings.getWidth()) / settings.getHeight();
	perspective = glm::perspective(glm::radians(settings.getFov()), aspectRatio, 0.1f, settings.getViewDistance());

	position = glm::vec3(0.0, 0.0, 0.0);
	lookAt = glm::vec3(0.0, 0.0, -1.0);
	view = glm::lookAt(
		position,
		lookAt + position,
		glm::vec3(0.0, 1.0, 0.0)
	);
}

void Camera::move(glm::vec3 movement)
{
	glm::vec3 forwardVector = normalize(glm::vec3(lookAt.x, 0.0, lookAt.z));
	glm::normalize(forwardVector);

	//moving forward
	position += movement[2] * forwardVector;

	//moving left
	leftVector = glm::cross(glm::vec3(0.0, 1.0, 0.0), forwardVector);
	position += movement[0] * leftVector;

	//moving up
	position += movement[1] * glm::vec3(0.0, 1.0, 0.0);

	view = glm::lookAt(
		position,
		lookAt + position,
		glm::vec3(0.0, 1.0, 0.0)
	);
}

void Camera::rotate(glm::vec2 rotation)
{
	glm::vec3 forwardVector = normalize(glm::vec3(lookAt.x, 0.0, lookAt.z));
	leftVector = glm::cross(glm::vec3(0.0, 1.0, 0.0), forwardVector);

	lookAt = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[0]), glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(lookAt, 0.0);
	lookAt = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[1]), normalize(leftVector)) * glm::vec4(lookAt, 0.0);

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

glm::vec3 Camera::getPosition()
{
	return position;
}
