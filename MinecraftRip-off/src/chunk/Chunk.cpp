#include "Chunk.h"
#include "../Block/Cube.h"
#include <iostream>

Chunk::Chunk(int x, int z)
	:
	xPosition{ x },
	zPosition{ z }
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
			auto coords = getCoords(i);
			temporaryBlocks.emplace_back(xPosition + std::get<0>(coords), std::get<1>(coords), zPosition + std::get<2>(coords), blocks[i]);
		}
	}

	requiresUpdate = false;
}

void Chunk::render(shader_prog& shader)
{
	for (auto& cube : temporaryBlocks)
	{
		cube.render();
	}
}

std::tuple<int, int, int> Chunk::getCoords(int index)
{
	int x = index % 16;
	int z = (index % 256) / 16;
	int y = index / 256;

	return std::tuple<int, int, int>(x, y, z);
}
