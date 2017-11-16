#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager()
{
	loadTextures();
	std::cout << "Textures loaded count: " << textureHandles.size() << std::endl;

	loadShaders();
	std::cout << "Shaders loaded count: " << shaderHandles.size() << std::endl;
}

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager resourceManager;
	return resourceManager;
}

void ResourceManager::loadTextures()
{
	textureHandles[Block::Dirt] = load_texture(GL_TEXTURE_2D, GL_RGB, "blocks/dirt.png");
	textureHandles[Block::Sand] = load_texture(GL_TEXTURE_2D, GL_RGB, "blocks/sand.png");
	textureHandles[Block::Grass] = load_texture(GL_TEXTURE_2D, GL_RGB, "blocks/grass.png");
	textureHandles[Block::Stone] = load_texture(GL_TEXTURE_2D, GL_RGB, "blocks/stone.png");
	textureHandles[Block::Wood] = load_texture(GL_TEXTURE_2D, GL_RGB, "blocks/wood.png");
	textureHandles[Block::Leaves] = load_texture(GL_TEXTURE_2D, GL_RGB, "blocks/leaves.png");
}

void ResourceManager::loadShaders()
{
	shader_prog shader = shader_prog{ "default.vert.glsl", "default.frag.glsl" };
	shader.use();
	shaderHandles.insert(std::make_pair(Shader::Default, shader));

	shader = shader_prog{ "texture.vert.glsl", "texture.frag.glsl" };
	shader.use();
	shaderHandles.insert(std::make_pair(Shader::Texture, shader));
}
