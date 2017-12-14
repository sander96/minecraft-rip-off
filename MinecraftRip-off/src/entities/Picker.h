#ifndef PICKER_H
#define PICKER_H
#pragma once

#include "../Camera.h"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"


class Picker
{
public:
	Picker();
	void setCamera(Camera cam) { camera = cam; };
	void setProjectionMat(glm::mat4 proj) { projectionMatrix = proj; };
	void setwindow(GLFWwindow* win) { window = win; };
	void update();

	glm::vec3 getCurrentRay() { return currentRay; };

	glm::vec3 calculateMouseRay();
private:
	glm::vec3 currentRay;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	Camera camera;
	GLFWwindow* window;
};

#endif

