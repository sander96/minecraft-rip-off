#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utilities/Shader_util.h"

class PlayState
{
public:
	PlayState();

	void processInput(GLFWwindow* window);
	void update(GLFWwindow* window);
	void render(GLFWwindow* window);

private:
	GLuint testTriangleVAO;	// remove this when not needed anymore
	shader_prog shader;	// temp, create shaderManager
};

#endif