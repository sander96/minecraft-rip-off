#include "PlayState.h"
#include "utilities/Shader_util.h"
#include <iostream>
#include "utilities/Settings.h"

PlayState::PlayState(GLFWwindow* window)
	:
	shader{ "default.vert.glsl", "default.frag.glsl" }
{
	shader.use();

	GLfloat vertices[] = {
		0.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
	};

	GLubyte indices[] = {
		0, 1, 2
	};

	GLfloat colors[] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};

	GLuint vertexArrayHandle;
	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);

	glfwGetCursorPos(window, &mousePosX, &mousePosY); // init global mouse positions

	shader.attribute3fv("position", vertices, 9);
	shader.attribute3fv("color", colors, 9);

	GLuint vboHandle;
	glGenBuffers(1, &vboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * 9, indices, GL_STATIC_DRAW);

	testTriangleVAO = vertexArrayHandle;
	glfwSetCursorPos(window, settings.getWidth() / 2.0, settings.getHeight() / 2.0);
}



void PlayState::processInput(GLFWwindow * window)
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
}

void PlayState::update(GLFWwindow* window)
{

}

void PlayState::render(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 matrix{ 1.0 };
	matrix = glm::scale(matrix, glm::vec3(0.5, 0.5, 0.5));
	//matrix = glm::rotate(matrix, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	shader.uniformMatrix4fv("modelMatrix", matrix);
	shader.uniformMatrix4fv("projectionMatrix", camera.getPerspective());
	shader.uniformMatrix4fv("viewMatrix", camera.getView());
	glBindVertexArray(testTriangleVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

	glfwSwapBuffers(window);
}
