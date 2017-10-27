#include "PlayState.h"

void PlayState::processInput(GLFWwindow* window)
{

}

void PlayState::update(GLFWwindow* window)
{

}

void PlayState::render(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();

	glfwSwapBuffers(window);

	glfwPollEvents();
}
