#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "utilities/Shader_util.h"
#include "utilities/Settings.h"
#include "utilities/ResourceManager.h"
#include "World.h"

class PlayState
{
public:
	explicit PlayState(GLFWwindow* window);

	void processInput(GLFWwindow* window);
	void update(GLFWwindow* window);
	void render(GLFWwindow* window);

private:
	Settings settings;
	ResourceManager* resourceManager;
	World world;
};

#endif
