#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Chunk.h"
#include <memory>
#include "../utilities/ResourceManager.h"
#include "ChunkCoordinate.h"

class ChunkManager
{
public:
	ChunkManager();

	void updateChunks(glm::vec3 playerPosition);
	std::unique_ptr<Chunk> createChunk(ChunkCoordinate coordinate);

	void updateMesh();
	void renderChunks();

private:
	std::multimap<ChunkCoordinate, std::unique_ptr<Chunk>> chunks;	// maybe some other data structure
	ChunkCoordinate previousPlayerPosition;
};

#endif
