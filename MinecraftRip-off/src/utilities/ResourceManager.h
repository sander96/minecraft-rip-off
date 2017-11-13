#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <GL/glew.h>
#include "Shader_util.h"
#include "Texture_util.h"

enum class Block
{
	Dirt, Sand, Grass, Stone, Wood, Leaves
};

enum class Shader
{
	Default, Texture
};


class ResourceManager
{
public:
	ResourceManager();

	const GLuint getTextureHandle(Block block) { return textureHandles[block]; }
	shader_prog& getShaderHandle(Shader shader) { return shaderHandles.find(shader)->second; }

private:
	void loadTextures();
	void loadShaders();

	std::map<Block, GLuint> textureHandles;
	std::map<Shader, shader_prog> shaderHandles;
};

#endif