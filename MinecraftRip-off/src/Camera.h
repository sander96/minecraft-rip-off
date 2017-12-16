#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera();

	void setPosition(glm::vec3 p) { position = p; }
	void setLookAt(glm::vec3 l) { lookAt = l; }
	void update();

	glm::mat4 getPerspective();
	glm::mat4 getView();
	glm::vec3 getPosition();

	// camera normal
	glm::vec3 getDirection();

private:
	glm::vec3 lookAt;
	glm::vec3 leftVector;
	glm::vec3 position;
	glm::mat4 perspective;
	glm::mat4 view;
};

#endif
