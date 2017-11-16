#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Chunk.h"
#include <memory>
#include "../utilities/ResourceManager.h"

class ChunkManager
{
public:
	ChunkManager();

	void updateMesh();
	void renderChunks();

private:
	std::map<std::pair<int, int>, std::unique_ptr<Chunk>> chunks;
};

#endif
