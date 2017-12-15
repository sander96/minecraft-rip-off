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
#include "../entities/Picker.h"

class ChunkManager
{
public:
	ChunkManager();

	void updateChunks(glm::vec3 playerPosition,int seis,glm::vec3 ray);
	std::unique_ptr<Chunk> createChunk(ChunkCoordinate coordinate);
	void renderChunks();

private:
	void ChunkManager::createCactus(int x, int y, int z, int size, std::unique_ptr<Chunk> const& chunk);
	void ChunkManager::createTree(int x, int y, int z, std::unique_ptr<Chunk> const& chunk);

	std::multimap<ChunkCoordinate, std::unique_ptr<Chunk>> chunks;
	ChunkCoordinate previousPlayerPosition;
	PerlinNoise biome;
	PerlinNoise perlinNoise;
	std::set<ChunkCoordinate> visitedChunks;
	
	Settings settings;
	const int radius;
	ConcurrencyManager concurrencyManager;
	Picker picker;
};

#endif
