#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <GLFW/glfw3.h>

class PlayState
{
public:
	void processInput(GLFWwindow* window);
	void update(GLFWwindow* window);
	void render(GLFWwindow* window);
};

#endif