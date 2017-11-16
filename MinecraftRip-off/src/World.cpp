#include "World.h"

World::World(GLFWwindow* win)
	: window{ win }
{

}

void World::update()
{
	player.processInput(window);
	chunkManager.updateMesh();
}

void World::render()
{
	shader_prog& shader = ResourceManager::getInstance().getShaderHandle(Shader::Texture);
	shader.uniformMatrix4fv("projectionMatrix", player.getCameraPerspective());
	shader.uniformMatrix4fv("viewMatrix", player.getCameraView());

	chunkManager.renderChunks();
}
