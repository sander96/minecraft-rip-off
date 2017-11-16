#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "../Camera.h"
#include "../utilities/Settings.h"

class Player
{
public:
	void processInput(GLFWwindow* window);

	glm::vec3 getPosition() { return position; }
	glm::mat4 getCameraPerspective() { return camera.getPerspective(); };
	glm::mat4 getCameraView() { return camera.getView(); }

private:
	glm::vec3 position;
	Camera camera;
	Settings settings;
};

#endif
