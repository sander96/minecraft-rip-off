#include "Cube.h"
#include "../utilities/ResourceManager.h"

Cube::Cube(int x, int y, int z, Block block)
	:
	x{ x },
	y{ y },
	z{ z },
	block{ block }
{
	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);
	glGenBuffers(1, &arrayBufferHandle);

	GLfloat vertexData[] = {
		//X		Y		Z		U		V					NORMAL
		// bottom
		-1.0f, -1.0f, -1.0f,    0.0f,0.0f,				0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		1.0f,0.0f,				0.0f,-1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,    0.0f,(float)1 / 3,		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,     1.0f,0.0f,				1.0f, -1.0f, 0.0f,
		1.0f, -1.0f,  1.0f,     1.0f,(float)1 / 3,		0.0f, -1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,    0.0f,(float)1 / 3,		0.0f, -1.0f, 0.0f,

		// top
		-1.0f, 1.0f, -1.0f,		0.0f,1.0f,				0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f,  1.0f,		0.0f,(float)2 / 3,		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		1.0f,1.0f,				0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		1.0f,1.0f,				0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f,  1.0f,		0.0f,(float)2 / 3,		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f,  1.0f,		1.0f,(float)2 / 3,	    0.0f, 1.0f, 0.0f,

		// front
		-1.0f, -1.0f, 1.0f,		0.0f,(float)1 / 3,		0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,		1.0f,(float)1 / 3,		0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,		0.0f,(float)2 / 3,	    0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,		1.0f,(float)1 / 3,		0.0f, 0.0f, 1.0f,
		1.0f,  1.0f, 1.0f,		1.0f,(float)2 / 3,		0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,		0.0f,(float)2 / 3,	    0.0f, 0.0f, 1.0f,

		// back
		-1.0f, -1.0f, -1.0f,	1.0f,(float)1 / 3,		0.0f, 0.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,	1.0f,(float)2 / 3,		0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, -1.0f,		0.0f,(float)1 / 3,		0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, -1.0f,		0.0f,(float)1 / 3,		0.0f, 0.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,	1.0f,(float)2 / 3,		0.0f, 0.0f, -1.0f,
		1.0f,  1.0f, -1.0f,		0.0f,(float)2 / 3,		0.0f, 0.0f, -1.0f,

		// left
		-1.0f, -1.0f,  1.0f,	1.0f,(float)1 / 3,		-1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f,(float)2 / 3,		-1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f,(float)1 / 3,		-1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,	1.0f,(float)1 / 3,		-1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	1.0f,(float)2 / 3,		-1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f,(float)2 / 3,		-1.0f, 0.0f, 0.0f,

		// right
		1.0f, -1.0f,  1.0f,		0.0f,(float)1 / 3,		1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		1.0f,(float)1 / 3,		1.0f, 0.0f, 0.0f,
		1.0f,  1.0f, -1.0f,		1.0f,(float)2 / 3,		1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f,		0.0f,(float)1 / 3,		1.0f, 0.0f, 0.0f,
		1.0f,  1.0f, -1.0f,		1.0f,(float)2 / 3,		1.0f, 0.0f, 0.0f,
		1.0f,  1.0f,  1.0f,		0.0f,(float)2 / 3,		1.0f, 0.0f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

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

Cube::~Cube()
{
	glDeleteVertexArrays(1, &vertexArrayHandle);
	glDeleteBuffers(1, &arrayBufferHandle);
}

void Cube::render()
{
	glm::mat4 matrix{ 1.0 };
	glm::vec3 test = glm::vec3(2 * x, 2 * y, 2 * z);
	matrix = glm::scale(matrix, glm::vec3(0.5));
	matrix = glm::translate(matrix, test);

	shader_prog& shader = ResourceManager::getInstance().getShaderHandle(Shader::Texture);

	shader.uniformMatrix4fv("modelMatrix", matrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::getInstance().getTextureHandle(block));

	glBindVertexArray(vertexArrayHandle);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
