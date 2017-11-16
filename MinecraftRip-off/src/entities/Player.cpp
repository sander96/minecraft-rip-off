#include "Player.h"

void Player::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera.move(glm::vec3(0.0, 0.0, 0.1));
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		camera.move(glm::vec3(0.1, 0.0, 0.0));
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera.move(glm::vec3(0.0, 0.0, -0.1));
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		camera.move(glm::vec3(-0.1, 0.0, 0.0));
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	double dX = (xpos - settings.getWidth() / 2.0) * settings.getSensitivity();
	double dY = (ypos - settings.getHeight() / 2.0)  * settings.getSensitivity();

	camera.rotate(glm::vec2(-dX, -dY));
	glfwSetCursorPos(window, settings.getWidth() / 2.0, settings.getHeight() / 2.0);

	position = camera.getPosition();
}
