#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <GL/glew.h>
#include "Shader_util.h"
#include "Texture_util.h"
#include "../block/Block.h"

enum class Shader
{
	Default, Texture
};


class ResourceManager
{
public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	static ResourceManager& getInstance();

	const GLuint getTextureHandle(Block block) { return textureHandles[block]; }
	shader_prog& getShaderHandle(Shader shader) { return shaderHandles.find(shader)->second; }

private:
	ResourceManager();

	void loadTextures();
	void loadShaders();

	std::map<Block, GLuint> textureHandles;
	std::map<Shader, shader_prog> shaderHandles;
};

#endif
