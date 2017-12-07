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
		0, 1, 2,
		0, 2, 3
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
	const int constant = 100;	// larger == entity moves slower

	if (tickCounter >= 360 * constant)
	{
		tickCounter = 0;
	}

	float arg = tickCounter * (1.0 / constant);
	float rad = static_cast<float>(glm::radians(arg + startDegrees));

	glm::mat4 matrix{ 1.0 };
	matrix = glm::rotate(matrix, rad, glm::vec3(0.0, 0.0, 1.0));

	glm::vec3 position = playerPosition + glm::vec3(800.0, 0.0, 0.0);
	matrix = glm::translate(matrix, position);

	matrix = glm::rotate(matrix, static_cast<float>(glm::radians(90.0)), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, static_cast<float>(glm::radians(180.0)), glm::vec3(1.0, 0.0, 0.0));

	matrix = glm::scale(matrix, glm::vec3(scale, scale, 1.0));

	modelMatrix = matrix;

	++tickCounter;
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
