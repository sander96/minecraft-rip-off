#include "ChunkManager.h"
#include "../Block/Cube.h"
#include <stdlib.h>

ChunkManager::ChunkManager()
	:
	previousPlayerPosition{ glm::vec3(123.0, 0.0, 0.0) },	// to force chunk update
	perlinNoise{}
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

		const int radius = 2;

		for (auto& pair : chunks)
		{
			if (playerChunkCoord.distance(pair.first) <= radius)
			{
				ChunkCoordinate position = pair.first;
				position.setDistance(playerChunkCoord.distance(position));

				newChunks.insert({ position, std::move(pair.second) });
			}
			else
			{
				pair.second->saveChunk();
			}
		}

		for (int i = -radius; i <= radius; ++i)
		{
			for (int j = -radius; j <= radius; ++j)
			{
				ChunkCoordinate position = glm::vec3(i * 16 + playerPosition[0], 0.0, j * 16 + playerPosition[2]);

				auto it = std::find_if(newChunks.begin(), newChunks.end(), [position](auto& pair) { return pair.first == position; });

				if (it != newChunks.end())
				{
					continue;
				}

				if (playerChunkCoord.distance(position) <= radius)
				{
					if (visitedChunks.find(position) == visitedChunks.end())	// chunk has never been loaded
					{
						position.setDistance(playerChunkCoord.distance(position));

						newChunks.insert({ position, createChunk(position) });
						visitedChunks.insert(position);
					}
					else
					{
						position.setDistance(playerChunkCoord.distance(position));
						auto visitedChunk = std::make_unique<Chunk>(position.getX() * 16, position.getZ() * 16);
						visitedChunk->loadChunk();
						newChunks.insert({ position, std::move(visitedChunk) });
					}
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

	for (int x = 0; x < 16; ++x)
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

			chunk->setBlock(Block::Grass, x, y1 + y2, z);
		}
	}

	chunk->setBlock(Block::Leaves, 6, 6, 6);

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
