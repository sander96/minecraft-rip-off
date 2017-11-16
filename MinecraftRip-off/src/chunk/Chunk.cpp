#include "Chunk.h"
#include "../Block/Cube.h"
#include <iostream>

Chunk::Chunk()
{
	blocks.fill(Block::Air);
}

void Chunk::setBlock(Block block, unsigned int x, unsigned int y, unsigned int z)
{
	blocks[x + 16 * z + 256 * y] = block;
	requiresUpdate = true;
}

void Chunk::updateMesh()	// temporary mesh logic, currently unefficient
{
	if (!requiresUpdate)
	{
		return;
	}

	temporaryBlocks.clear();

	for (int i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i] != Block::Air)
		{
			temporaryBlocks.push_back(std::pair<GLuint, int>(makeBlock(blocks[i]), i));
		}
	}

	requiresUpdate = false;
}

void Chunk::render(shader_prog& shader)
{
	for (auto pair : temporaryBlocks)
	{
		auto coords = getCoords(pair.second);
		renderCube(pair.first, shader, std::get<0>(coords), std::get<1>(coords), std::get<2>(coords), blocks[pair.second]);
	}
}

std::tuple<int, int, int> Chunk::getCoords(int index)
{
	int x = index % 16;
	int z = (index % 256) / 16;
	int y = index / 256;

	return std::tuple<int, int, int>(x, y, z);
}
