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
	textureHandles[Texture::Atlas] = load_texture(GL_TEXTURE_2D, GL_RGB, "blocks/texture_atlas.png");
	textureHandles[Texture::Sun] = load_texture(GL_TEXTURE_2D, GL_RGB, "other/sun.png");
	textureHandles[Texture::Moon] = load_texture(GL_TEXTURE_2D, GL_RGB, "other/moon.png");
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
