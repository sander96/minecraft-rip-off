#ifndef CHUNK_H
#define CHUNK_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include "../Block/Block.h"
#include "../utilities/Shader_util.h"
#include "ChunkMesh.h"
#include "ChunkCoordinate.h"
#include <atomic>

class Chunk
{
public:
	Chunk(int x, int z);

	void setBlock(Block block, unsigned int x, unsigned int y, unsigned int z);

	void generateMesh();
	void updateMesh();

	void saveChunk();
	void loadChunk();

	bool requiresMeshUpdate() { return meshUpdateFlag; }

	void meshUpdateFlagOff() { meshUpdateFlag = false; }

	void deactivate() { active = false; }

	bool isActive() { return active; }

	void render(shader_prog& shader);

private:
	std::tuple<int, int, int> getCoords(int index);

	int xPosition;
	int zPosition;
	ChunkCoordinate chunkCoord;

	std::atomic<bool> active = true;

	std::atomic<bool> meshUpdateFlag = true;

	std::atomic<bool> requiresUpdate = true;


	bool renderLocked = true;

	std::array<Block, 16 * 16 * 256> blocks;
	ChunkMesh chunkMesh;
};

#endif
