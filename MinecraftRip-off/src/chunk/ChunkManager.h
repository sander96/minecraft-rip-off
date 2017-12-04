#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Chunk.h"
#include <memory>
#include "../utilities/ResourceManager.h"
#include "../utilities/PerlinNoise.h"
#include "ChunkCoordinate.h"
#include <set>
#include "../utilities/ConcurrencyManager.h"
#include "../utilities/Settings.h"

class ChunkManager
{
public:
	ChunkManager();

	void updateChunks(glm::vec3 playerPosition);
	std::unique_ptr<Chunk> createChunk(ChunkCoordinate coordinate);

	void renderChunks();

private:
	std::multimap<ChunkCoordinate, std::unique_ptr<Chunk>> chunks;
	ChunkCoordinate previousPlayerPosition;
	PerlinNoise biome;
	PerlinNoise perlinNoise;
	std::set<ChunkCoordinate> visitedChunks;

	Settings settings;
	const int radius;
	ConcurrencyManager concurrencyManager;
};

#endif
