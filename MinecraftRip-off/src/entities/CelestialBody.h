#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class CelestialBody
{
public:
	CelestialBody();
	~CelestialBody();

	void update(glm::vec3 playerPosition);
	void render();

protected:
	void setTextureHandle(GLuint textureHandle) { this->textureHandle = textureHandle; }
	void setStartDegrees(float degrees) { startDegrees = degrees; }
	void setScale(float scale) { this->scale = scale; }

private:
	glm::mat4 modelMatrix;
	GLuint vertexArrayHandle;

	GLuint arrayBufferHandle;
	GLuint indicesHandle;

	GLuint textureHandle;

	float startDegrees;
	float scale;

	unsigned int tickCounter = 0;
};

#endif