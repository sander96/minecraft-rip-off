#include "World.h"

World::World(GLFWwindow* win)
	:
	window{ win },
	player{ glm::vec3(0.0, 50.0, 0.0) }
{

}

void World::update()
{
	player.processInput(window);

	sun.update(player.getPosition());
	moon.update(player.getPosition());

	chunkManager.updateChunks(player);
}

void World::render()
{
	shader_prog& shader = ResourceManager::getInstance().getShaderHandle(Shader::Texture);
	shader.uniformMatrix4fv("projectionMatrix", player.getCameraPerspective());
	shader.uniformMatrix4fv("viewMatrix", player.getCameraView());

	sun.render();
	moon.render();

	chunkManager.renderChunks();
}
