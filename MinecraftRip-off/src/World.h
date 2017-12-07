#ifndef WORLD_H
#define WORLD_H

#include "chunk/ChunkManager.h"
#include "entities/Player.h"
#include "entities/Sun.h"
#include "entities/Moon.h"

class World
{
public:
	explicit World(GLFWwindow* win);

	void update();

	void render();

private:
	GLFWwindow* window;
	ChunkManager chunkManager;

	Player player;
	Sun sun;
	Moon moon;
};

#endif
