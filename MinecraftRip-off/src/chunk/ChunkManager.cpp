#include "ChunkManager.h"
#include "../Block/Cube.h"
#include <stdlib.h>

ChunkManager::ChunkManager()
	:
	previousPlayerPosition{ glm::vec3(123.0, 0.0, 0.0) },	// to force chunk update
	perlinNoise {}
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

		const int radius = 6;	// currently low because of performance issues

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
			// High frequency noise
			double i1 = (coordinate.getX() * 16.0 + x) / 16.0;
			double j1 = (coordinate.getZ() * 16.0 + z) / 16.0;
			double y1 = 15 * perlinNoise.noise(i1, j1, 0.8);

			// Lower frequency noise
			double i2 = (coordinate.getX() * 16.0 + x) / 64.0;
			double j2 = (coordinate.getZ() * 16.0 + z) / 64.0;
			double y2 = 40 * perlinNoise.noise(i2, j2, 0.8);

			chunk->setBlock(Block::Grass, x, y1+y2, z);
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
