#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include "utilities/Shader_util.h"
#include "Camera.h"
#include "utilities/Settings.h"
#include "utilities/ResourceManager.h"

class PlayState
{
public:
	PlayState(GLFWwindow* window);

	void processInput(GLFWwindow* window);
	void update(GLFWwindow* window);
	void render(GLFWwindow* window);

private:
	Camera camera;
	double mousePosX;
	double mousePosY;
	Settings settings;
	ResourceManager resourceManager;
};

#endif