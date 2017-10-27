#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLFW/glfw3.h>
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