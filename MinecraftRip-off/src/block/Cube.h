#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../utilities/Shader_util.h"
#include "Block.h"

GLuint makeBlock(Block block);
void renderCube(const GLuint handle, shader_prog& shader, int x, int y, int z, Block block);

#endif
