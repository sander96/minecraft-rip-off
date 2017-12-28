#include "ChunkManager.h"
#include "../Block/Cube.h"
#include <stdlib.h>


ChunkManager::ChunkManager()
	:
	previousPlayerPosition{ glm::vec3(123.0, 0.0, 0.0) },	// to force chunk update
	radius{ settings.getChunkRadius() },
	biome{ PerlinNoise::PerlinNoise(100) }
{

}

void ChunkManager::updateChunks(Player& player)
{
	auto playerPosition = player.getPosition();
	ChunkCoordinate playerChunkCoord{ playerPosition };

	if (playerChunkCoord != previousPlayerPosition)		// true if player entered to a new chunk
	{
		std::cout << "Player entered to a new chunk" << std::endl;	// debugging info
		std::multimap<ChunkCoordinate, std::unique_ptr<Chunk>> newChunks;

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

	rayCast(player);

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
	int currentNumOfTrees = 0;

	for (int x = 0; x < 16; ++x)
	{
		for (int z = 0; z < 16; ++z)
		{
			double worldX = (coordinate.getX() * 16.0 + x);
			double worldZ = (coordinate.getZ() * 16.0 + z);
			// High frequency noise
			double i1 = worldX / 16.0;
			double j1 = worldZ / 16.0;
			double y1 = 15 * perlinNoise.noise(i1, j1, 0.8);

			// Lower frequency noise
			double i2 = worldX / 64.0;
			double j2 = worldZ / 64.0;
			double y2 = 40 * perlinNoise.noise(i2, j2, 0.8);

			// Mountains
			double i3 = worldX / 64.0;
			double j3 = worldZ / 64.0;
			double y3 = max(0.0, perlinNoise.noise(i3, j3, 0.8) - 0.6) * 100.0;

			// Noise for different biomes
			double b1 = worldX / 200.0;
			double b2 = worldZ / 200.0;
			double biomeType = biome.noise(b1, b2, 0.8);

			if (y1 + y2 + y3 < 20)
			{
				for (int i = y1 + y2 + y3; i <= 20; i++)
				{
					chunk->setBlock(Block::Water, x, i, z);
				}

				for (int i = 0; i < y1 + y2 + y3 - 1; i++)
				{
					chunk->setBlock(Block::Dirt, x, i, z);
				}
			}
			else
			{
				if (biomeType > 0.6)
				{
					int c1 = worldX;
					int c2 = worldZ;
					if (y1 + y2 + y3 < 26 && (c1 * c2) % 53 == 12)
					{
						createCactus(x, y1 + y2 + y3, z, 5, chunk);
					}

					for (int i = 0; i <= y1 + y2 + y3; i++)
					{
						chunk->setBlock(Block::Sand, x, i, z);
					}
				}
				else
				{
					double t1 = worldX / 25.0;
					double t2 = worldZ / 25.0;
					double treeNoise = 10.0 * (perlinNoise.noise(t1, t2, 0.8) - 0.5);

					std::vector<int> treeX = {5, 6, 8, 9, 11};
					std::vector<int> treeZ = {5, 12, 8, 11, 7};

					if (x == treeX[currentNumOfTrees] && z == treeZ[currentNumOfTrees] && currentNumOfTrees < treeNoise)
					{
						createTree(treeX[currentNumOfTrees], y1 + y2 + y3, treeZ[currentNumOfTrees], chunk);
						currentNumOfTrees++;
					}

					for (int i = 0; i < y1 + y2 + y3; i++)
					{
						chunk->setBlock(Block::Dirt, x, i, z);
					}
					chunk->setBlock(Block::Grass, x, y1 + y2 + y3, z);
				}
			}
		}
	}
	return chunk;
}

void ChunkManager::rayCast(Player& player)
{
	auto playerPosition = player.getPosition();
	auto ray = player.getCameraDirection();
	double step = 0.1;
	int maxDistance = 4;
	auto trace = glm::vec3(0.0, 0.0, 0.0);

	if (player.addBlockEvent() || player.removeBlockEvent())
	{
		int previousX = static_cast<int>(playerPosition.x) % 16;
		int previousY = static_cast<int>(playerPosition.y) % 256;
		int previousZ = static_cast<int>(playerPosition.z) % 16;

		if (previousX < 0)
		{
			previousX += 15;
		}

		if (previousZ < 0)
		{
			previousZ += 15;
		}

		std::unique_ptr<Chunk>* previousChunk = nullptr;

		for (double c = 0.0; glm::length(trace) <= maxDistance; c += step)
		{
			trace = glm::vec3(ray.x * c, ray.y * c, ray.z * c);
			auto tracePosition = playerPosition + trace;

			auto chunk = worldCoordToChunk(tracePosition);

			if (chunk != nullptr)
			{
				if (tracePosition.y < 0 || tracePosition.y >= 256)
				{
					continue;
				}

				int x = static_cast<int>(tracePosition.x) % 16;
				int y = static_cast<int>(tracePosition.y) % 256;
				int z = static_cast<int>(tracePosition.z) % 16;

				if (tracePosition.x < 0)
				{
					x += 15;
				}

				if (tracePosition.z < 0)
				{
					z += 15;
				}

				if (player.addBlockEvent())
				{
					if (previousX == x && previousY == y && previousZ == z)
					{
						continue;
					}

					if ((*chunk)->getBlock(x, y, z) != Block::Air)
					{
						if (previousChunk)
						{
							Block currentBlock = player.getCurrentBlock();

							concurrencyManager.addSetBlockTask([previousChunk, currentBlock, previousX, previousY, previousZ]() {
								(*previousChunk)->setBlock(currentBlock, previousX, previousY, previousZ);
							});
						}
						break;
					}
					else
					{
						previousX = x;
						previousY = y;
						previousZ = z;
						previousChunk = worldCoordToChunk(tracePosition);
					}
				}
				else
				{
					if ((*chunk)->getBlock(x, y, z) != Block::Air)
					{
						concurrencyManager.addSetBlockTask([chunk, x, y, z]() {
							(*chunk)->setBlock(Block::Air, x, y, z);
						});
						break;
					}
				}
			}
		}
	}
}

// check for nullptr
std::unique_ptr<Chunk>* ChunkManager::worldCoordToChunk(glm::vec3 worldCoordinate)
{
	ChunkCoordinate chunkCoordinate = worldCoordinate;

	auto iterator = chunks.find(worldCoordinate);

	if (iterator == chunks.end())
	{
		return nullptr;
	}

	return &(iterator->second);
}

void ChunkManager::createCactus(int x, int y, int z, int size, const std::unique_ptr<Chunk>& chunk)
{
	for (int i = 1; i < size + 1; i++)
	{
		chunk->setBlock(Block::Cactus, x, y + i, z);
	}
}

void ChunkManager::createTree(int x, int y, int z, const std::unique_ptr<Chunk>& chunk)
{
	//SIZE peab olema kuidagi seotud noisega, siis suvalise suurusega puud..
	int size = 5 + y % 4;
	//tüvi
	for (int i = 1; i < size + 1; i++)
	{
		chunk->setBlock(Block::Wood, x, y + i, z);
	}
	//lehed ümber?
	int muutuja = 2;
	if (size > 5) {
		muutuja = 3;
	}
	for (int i = 0; i < size / 2 + 1; i++)
	{
		if (i == size / 2 - 1)
		{
			muutuja = 2;
		}
		if (i == size / 2)
		{
			for (int p = -muutuja + 1; p < muutuja; p++)
			{
				chunk->setBlock(Block::Leaves, x, y + size - 1 + i, z + p);
				chunk->setBlock(Block::Leaves, x + p, y + size - 1 + i, z);
			}
		}
		else
		{
			for (int j = -muutuja + 1; j < muutuja; j++)
			{
				for (int p = -muutuja + 1; p < muutuja; p++)
				{
					if (j != 0 || p != 0 || i > 0)
					{
						chunk->setBlock(Block::Leaves, x + j, y + size - 1 + i, z + p);

					}
				}
			}
			if (muutuja == 3 && i == size / 2 - 2)
			{
				chunk->setBlock(Block::Air, x + muutuja - 1, y + size - 1 + i, z + muutuja - 1);
				chunk->setBlock(Block::Air, x - muutuja + 1, y + size - 1 + i, z + muutuja - 1);
				chunk->setBlock(Block::Air, x + muutuja - 1, y + size - 1 + i, z - muutuja + 1);
				chunk->setBlock(Block::Air, x - muutuja + 1, y + size - 1 + i, z - muutuja + 1);
			}
		}
	}
	chunk->setBlock(Block::Leaves, x, y + size + size / 2, z);
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
