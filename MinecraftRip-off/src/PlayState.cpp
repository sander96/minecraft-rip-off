#include "PlayState.h"
#include "utilities/Shader_util.h"
#include "utilities/Texture_util.h"
#include <iostream>
#include "utilities/Settings.h"
#include "block/Block.h"

PlayState::PlayState(GLFWwindow* window)
	: world{ window }
{
	resourceManager = &ResourceManager::getInstance();
	glfwSetCursorPos(window, settings.getWidth() / 2.0, settings.getHeight() / 2.0);
}


void PlayState::processInput(GLFWwindow* window)
{

}

void PlayState::update(GLFWwindow* window)
{
	world.update();
}

void PlayState::render(GLFWwindow* window)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	world.render();

	glfwSwapBuffers(window);
}
