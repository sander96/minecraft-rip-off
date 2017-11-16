#ifndef WORLD_H
#define WORLD_H

#include "chunk/ChunkManager.h"
#include "entities/Player.h"

class World
{
public:
	World(GLFWwindow* win);

	void update();

	void render();

private:
	ChunkManager chunkManager;
	Player player;
	GLFWwindow* window;
};

#endif
