#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../utilities/Shader_util.h"
#include "Block.h"

class Cube
{
public:
	Cube(int x, int y, int z, Block block);
	~Cube();

	void render();

private:
	GLuint vertexArrayHandle;
	GLuint arrayBufferHandle;

	Block block;

	int x;
	int y;
	int z;
};

#endif
