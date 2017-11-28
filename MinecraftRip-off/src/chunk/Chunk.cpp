#include "Chunk.h"
#include "../Block/Cube.h"
#include <iostream>

Chunk::Chunk(int x, int z)
	:
	xPosition{ x },
	zPosition{ z },
	chunkMesh{ x, z }
{
	blocks.fill(Block::Air);
}

void Chunk::setBlock(Block block, unsigned int x, unsigned int y, unsigned int z)
{
	blocks[x + 16 * z + 256 * y] = block;
	requiresUpdate = true;
}

void Chunk::updateMesh()
{
	if (!requiresUpdate)
	{
		return;
	}

	chunkMesh.updateChunkMesh(blocks);

	requiresUpdate = false;
}

void Chunk::render(shader_prog& shader)
{
	chunkMesh.render();
}

std::tuple<int, int, int> Chunk::getCoords(int index)
{
	int x = index % 16;
	int z = (index % 256) / 16;
	int y = index / 256;

	return std::tuple<int, int, int>(x, y, z);
}
