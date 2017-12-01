#include "Chunk.h"
#include "../Block/Cube.h"
#include <iostream>
#include <fstream>
#include <filesystem>

Chunk::Chunk(int x, int z)
	:
	xPosition{ x },
	zPosition{ z },
	chunkCoord{ glm::vec3(x, 0.0, z) },
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

void Chunk::saveChunk()
{
	std::experimental::filesystem::create_directory(std::experimental::filesystem::path{ "map" });

	std::string filename{ std::to_string(chunkCoord.getX()) + "_" + std::to_string(chunkCoord.getZ()) + ".bin" };
	std::ofstream outFile{ "map/" + filename, std::ios::binary };

	uint8_t counter = 0;
	Block currentBlock = Block::NULL_BLOCK;

	for (Block block : blocks)
	{
		if (block == currentBlock)
		{
			if (counter == UINT8_MAX)
			{
				outFile << static_cast<uint8_t>(Block::NULL_BLOCK) << counter << static_cast<uint8_t>(currentBlock);
				counter = 0;
			}

			++counter;
		}
		else
		{
			if (counter > 3)
			{
				outFile << static_cast<uint8_t>(Block::NULL_BLOCK) << counter << static_cast<uint8_t>(currentBlock);
			}
			else
			{
				for (int i = 0; i < counter; ++i)
				{
					outFile << static_cast<uint8_t>(currentBlock);
				}
			}

			currentBlock = block;
			counter = 1;
		}
	}

	if (counter > 3)
	{
		outFile << static_cast<uint8_t>(Block::NULL_BLOCK) << counter << static_cast<uint8_t>(currentBlock);
	}
	else
	{
		for (int i = 0; i < counter; ++i)
		{
			outFile << static_cast<uint8_t>(currentBlock);
		}
	}
}

void Chunk::loadChunk()
{
	std::string filename{ std::to_string(chunkCoord.getX()) + "_" + std::to_string(chunkCoord.getZ()) + ".bin" };
	std::ifstream inFile{ "map/" + filename, std::ios::binary };

	inFile >> std::noskipws;

	int index = 0;
	uint8_t input;

	while (inFile >> input)		// might be slow
	{
		if (input == UINT8_MAX)
		{
			uint8_t count, block;
			inFile >> count;
			inFile >> block;

			for (int i = 0; i < count; ++i)
			{
				blocks[index] = static_cast<Block>(block);
				++index;
			}
		}
		else
		{
			blocks[index] = static_cast<Block>(input);
			++index;
		}
	}
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
