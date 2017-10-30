#include "PlayState.h"
#include "utilities/Shader_util.h"

PlayState::PlayState()
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

	shader.attribute3fv("position", vertices, 9);
	shader.attribute3fv("color", colors, 9);

	GLuint vboHandle;
	glGenBuffers(1, &vboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * 9, indices, GL_STATIC_DRAW);

	testTriangleVAO = vertexArrayHandle;
}

void PlayState::processInput(GLFWwindow* window)
{
	glfwPollEvents();
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
	glBindVertexArray(testTriangleVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

	glfwSwapBuffers(window);
}
