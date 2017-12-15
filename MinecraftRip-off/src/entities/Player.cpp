#include "Player.h"
#include <iostream>
#include "../chunk/chunk.h"
#include "../chunk/ChunkManager.h"

Player::Player(glm::vec3 pos)
{
	position = pos;
	lookAt = glm::vec3(0.0, 0.0, -1.0);
}

void Player::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		move(glm::vec3(0.0, 0.0, 0.3));
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		move(glm::vec3(0.1, 0.0, 0.0));
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		move(glm::vec3(0.0, 0.0, -0.1));
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		move(glm::vec3(-0.1, 0.0, 0.0));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		move(glm::vec3(0.0, 0.1, 0.0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		move(glm::vec3(0.0, -0.1, 0.0));
	}
	
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	double dX = (xpos - settings.getWidth() / 2.0) * settings.getSensitivity();
	double dY = (ypos - settings.getHeight() / 2.0)  * settings.getSensitivity();

	rotate(glm::vec2(-dX, dY));
	glfwSetCursorPos(window, settings.getWidth() / 2.0, settings.getHeight() / 2.0);
	camera.setPosition(position);
	camera.setLookAt(lookAt);
	camera.update();
	picker.setCamera(camera);
	picker.setProjectionMat(camera.getPerspective());
	picker.setwindow(window);
	picker.update();
	//ray näitab umbes asukohta ja kui ntks kaugusel mängijast +3 on mingi asi, siis eemalda või lisa
	//mingi jama sellega ikka, kuidagi mitu klikki võtab..
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		std::cout << "lisan" << std::endl;
		glm::vec3 valja = picker.getCurrentRay();
		SetRay(valja);
		SetLisamine();
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		std::cout << "kustutan" << std::endl;
		glm::vec3 valja = picker.getCurrentRay();
		SetRay(valja);
		SetEemaldus();
	}

		
}

void Player::move(glm::vec3 movement)
{
	glm::vec3 forwardVector = normalize(glm::vec3(lookAt.x, 0.0, lookAt.z));
	glm::normalize(forwardVector);

	//moving forward
	position += movement[2] * forwardVector;

	//moving left
	leftVector = glm::cross(glm::vec3(0.0, 1.0, 0.0), forwardVector);
	position += movement[0] * leftVector;

	//moving up
	position += movement[1] * glm::vec3(0.0, 1.0, 0.0);
}

void Player::rotate(glm::vec2 rotation)
{
	glm::vec3 forwardVector = normalize(glm::vec3(lookAt.x, 0.0, lookAt.z));
	leftVector = glm::cross(glm::vec3(0.0, 1.0, 0.0), forwardVector);

	lookAt = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[0]), glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(lookAt, 0.0);

	if (rotation[1] < 0 && lookAt.y < 0.98) 
	{

		lookAt = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[1]), normalize(leftVector)) * glm::vec4(lookAt, 0.0);
	}
	else if (rotation[1] > 0 && lookAt.y > -0.98)
	{
		lookAt = glm::rotate(glm::mat4(1.0f), glm::radians(rotation[1]), normalize(leftVector)) * glm::vec4(lookAt, 0.0);
	}
}
