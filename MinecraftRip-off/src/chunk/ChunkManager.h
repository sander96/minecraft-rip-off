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
#include "../entities/Player.h"

class ChunkManager
{
public:
	ChunkManager();

	void updateChunks(Player& player);
	std::unique_ptr<Chunk> createChunk(ChunkCoordinate coordinate);
	void renderChunks();

private:
	void rayCast(Player& player);

	std::unique_ptr<Chunk>* worldCoordToChunk(glm::vec3 worldCoordinate);

	void ChunkManager::createCloud(const std::unique_ptr<Chunk>& chunk);
	void ChunkManager::createCactus(int x, int y, int z, const std::unique_ptr<Chunk>& chunk);
	void ChunkManager::createTree(int x, int y, int z, const std::unique_ptr<Chunk>& chunk);

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
