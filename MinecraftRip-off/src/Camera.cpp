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

void Camera::update()
{
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

glm::vec3 Camera::getDirection()
{
	return -glm::vec3(glm::inverse(view)[2]);
}
