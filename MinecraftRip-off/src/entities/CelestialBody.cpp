#include "CelestialBody.h"
#include "../utilities/ResourceManager.h"

CelestialBody::CelestialBody()
{
	GLfloat vertexData[] = {
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		1.0f,  1.0f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,	    0.0f, 0.0f, 1.0f,
	};

	GLuint indices[] = {
		1, 0, 2,
		2, 0, 3
	};

	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);

	glGenBuffers(1, &arrayBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &indicesHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	shader_prog& shader = ResourceManager::getInstance().getShaderHandle(Shader::Texture);

	GLuint loc = glGetAttribLocation(shader.getProg(), "position");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(0 * sizeof(float)));

	loc = glGetAttribLocation(shader.getProg(), "uv");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));

	loc = glGetAttribLocation(shader.getProg(), "normal");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(5 * sizeof(float)));

	glBindVertexArray(0);
}

CelestialBody::~CelestialBody()
{
	glDeleteVertexArrays(1, &vertexArrayHandle);
	glDeleteBuffers(1, &arrayBufferHandle);
	glDeleteBuffers(1, &indicesHandle);
}

void CelestialBody::update(glm::vec3 playerPosition)
{
	startDegrees += 0.01;
	if (startDegrees > 360)
	{
		startDegrees = 0.0;
	}

	double objY = sin(glm::radians(startDegrees)) * 800.0;
	double objZ = cos(glm::radians(startDegrees)) * 800.0;

	glm::mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, playerPosition + glm::vec3(0.0, objY, objZ));
	transform = glm::rotate(transform, glm::radians(-startDegrees), glm::vec3(1.0, 0.0, 0.0));
	transform = glm::scale(transform, glm::vec3(scale, scale, 1.0));

	modelMatrix = transform;
}

void CelestialBody::render()
{
	shader_prog& shader = ResourceManager::getInstance().getShaderHandle(Shader::Texture);
	shader.uniformMatrix4fv("modelMatrix", modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glBindVertexArray(vertexArrayHandle);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
