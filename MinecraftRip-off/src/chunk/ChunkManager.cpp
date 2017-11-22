#include "ChunkManager.h"
#include "../Block/Cube.h"

ChunkManager::ChunkManager()
	:
	previousPlayerPosition{ glm::vec3(123.0, 0.0, 0.0) }	// to force chunk update
{
	updateChunks(glm::vec3(0.0, 0.0, 0.0));
}

void ChunkManager::updateChunks(glm::vec3 playerPosition)
{
	ChunkCoordinate playerChunkCoord{ playerPosition };

	if (playerChunkCoord != previousPlayerPosition)		// true if player entered to a new chunk
	{
		std::cout << "Player entered to a new chunk" << std::endl;	// debugging info

		std::multimap<ChunkCoordinate, std::unique_ptr<Chunk>> newChunks;

		const int radius = 2;	// currently low because of performance issues

		for (int i = -radius; i <= radius; ++i)
		{
			for (int j = -radius; j <= radius; ++j)
			{
				ChunkCoordinate position = glm::vec3(i * 16 + playerPosition[0], 0.0, j * 16 + playerPosition[2]);

				if (playerChunkCoord.distance(position) <= radius)
				{
					position.setDistance(playerChunkCoord.distance(position));
					newChunks.insert({ position, createChunk(position) });
				}
			}
		}

		chunks = std::move(newChunks);
		previousPlayerPosition = playerChunkCoord;
		updateMesh();
	}
}

std::unique_ptr<Chunk> ChunkManager::createChunk(ChunkCoordinate coordinate)
{
	std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(coordinate.getX() * 16, coordinate.getZ() * 16);

	for (int x = 0; x < 16; ++x)		// temp
	{
		for (int z = 0; z < 16; ++z)
		{
			for (int y = 0; y < 3; ++y)		// currently low because of performance issues
			{
				if (y == 2)
				{
					chunk->setBlock(Block::Grass, x, y, z);
				}
				else if (y == 1)
				{
					chunk->setBlock(Block::Dirt, x, y, z);
				}
				else
				{
					chunk->setBlock(Block::Stone, x, y, z);
				}
			}
		}
	}

	return chunk;
}

void ChunkManager::updateMesh()
{
	for (auto& chunk : chunks)
	{
		chunk.second->updateMesh();
	}
}

void ChunkManager::renderChunks()
{
	for (auto& chunk : chunks)
	{
		chunk.second->render(ResourceManager::getInstance().getShaderHandle(Shader::Texture));
	}
}
