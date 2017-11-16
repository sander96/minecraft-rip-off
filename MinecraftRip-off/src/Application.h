#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include "PlayState.h"

class Application
{
public:
	Application(GLFWwindow* windowHandle);
	void run();

private:
	GLFWwindow* window;
	PlayState playState;
};

#endif
