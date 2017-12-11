#include "ChunkManager.h"
#include "../Block/Cube.h"
#include <stdlib.h>

ChunkManager::ChunkManager()
	:
	previousPlayerPosition{ glm::vec3(123.0, 0.0, 0.0) },	// to force chunk update
	radius{ settings.getChunkRadius() },
	biome{ PerlinNoise::PerlinNoise(100) }
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

		chunks.insert({ playerChunkCoord, createChunk(playerChunkCoord) });

		for (auto& pair : chunks)
		{
			if (playerChunkCoord.distance(pair.first) > radius)
			{
				concurrencyManager.addWriteTask([&pair]() {pair.second->saveChunk(); });
				pair.second->deactivate();
			}
		}

		for (int i = -radius; i <= radius; ++i)
		{
			for (int j = -radius; j <= radius; ++j)
			{
				ChunkCoordinate position = glm::vec3(i * 16 + playerPosition[0], 0.0, j * 16 + playerPosition[2]);

				auto it = std::find_if(chunks.begin(), chunks.end(), [position](auto& pair) { return pair.first == position; });

				if (it != chunks.end())
				{
					continue;
				}

				if (playerChunkCoord.distance(position) <= radius)
				{
					if (visitedChunks.find(position) == visitedChunks.end())	// chunk has never been loaded
					{
						chunks.insert({ position, createChunk(position) });
						visitedChunks.insert(position);
					}
					else
					{
						auto visitedChunk = std::make_unique<Chunk>(position.getX() * 16, position.getZ() * 16);
						auto it = chunks.insert({ position, std::move(visitedChunk) });
						concurrencyManager.addReadTask([it]() {it->second->loadChunk(); });
					}
				}
			}
		}

		previousPlayerPosition = playerChunkCoord;
	}

	for (auto& pair : chunks)
	{
		if (pair.second->requiresMeshUpdate())
		{
			pair.second->meshUpdateFlagOff();
			concurrencyManager.addChunkMeshTask([&pair]() {pair.second->generateMesh(); });
		}

		pair.second->updateMesh();
	}

	concurrencyManager.updateTasks();

	if (concurrencyManager.empty())
	{
		for (auto it = chunks.begin(); it != chunks.end();)
		{
			if (!(it->second->isActive()))
			{
				it = chunks.erase(it);
			}
			else
			{
				it = std::next(it);
			}
		}
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

			// Mountains
			double i3 = (coordinate.getX() * 16.0 + x) / 64.0;
			double j3 = (coordinate.getZ() * 16.0 + z) / 64.0;
			double y3 = max(0.0, perlinNoise.noise(i3, j3, 0.8) - 0.6) * 100.0;

			// Noise for different biomes
			double b1 = (coordinate.getX() * 16.0 + x) / 200.0;
			double b2 = (coordinate.getZ() * 16.0 + z) / 200.0;
			double biomeType = biome.noise(b1, b2, 0.8);

			if (y1 + y2 + y3 < 20)
			{
				chunk->setBlock(Block::Water, x, 20, z);
			}
			else
			{
				if (biomeType > 0.6)
				{
					chunk->setBlock(Block::Sand, x, y1 + y2 + y3, z);
				}
				else
				{
					chunk->setBlock(Block::Grass, x, y1 + y2 + y3, z);
				}
			}
		}
	}

	return chunk;
}

void ChunkManager::renderChunks()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::getInstance().getTextureHandle(Texture::Atlas));

	for (auto& chunk : chunks)
	{
		chunk.second->render(ResourceManager::getInstance().getShaderHandle(Shader::Texture));
	}
}
