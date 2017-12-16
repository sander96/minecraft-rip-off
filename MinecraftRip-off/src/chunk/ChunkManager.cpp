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
					if (x == 2 && z == 3)
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
					if (x == 2 && z == 3 || x == 8 && z == 6)
					{
						createTree(x, y1 + y2 + y3, z, chunk);
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
	ChunkCoordinate playerChunkCoord{ playerPosition };
	auto ray = player.getRay();

	int maxDistance = 4;

	if (player.addBlockEvent())		// TODO
	{
		std::cout << "Adding a block" << std::endl;	// remove this when removing/adding works

		//auto& currChunk = chunks.find(playerChunkCoord);
		////kui kaugule saame blokke panna
		//int kaugus = 4;
		////et ei paneks lambist õhku blokki, siis kontroll, et temast tagapool ikka on blokk..
		//std::cout << ray.x << "," << ray.y << "," << ray.z << std::endl;
		//int temp = 0;
		//bool found = false;
		////kas on kuni kauguseni vaba(et ei paneks teiste blokkide taha, vaid ette)
		//for (int i = 0; i <= kaugus; i++) {
		//	if (currChunk->second->getBlock(playerPosition.x + ray.x*i, playerPosition.y + ray.y*i, playerPosition.z + ray.z*i) != Block::Air) {
		//		temp = i - 1;
		//		found = true;
		//	}
		//}
		//if (found == true) {
		//	kaugus = temp;
		//}
		////selleks,et lampi õhku ei saaks blokke riputada..
		//if (currChunk->second->getBlock(playerPosition.x + ray.x*(kaugus + 1), playerPosition.y + ray.y*(kaugus + 1), playerPosition.z + ray.z*(kaugus + 1)) != Block::Air) {
		//	currChunk->second->setBlock(Block::Stone, playerPosition.x + ray.x*kaugus, playerPosition.y + ray.y*kaugus, playerPosition.z + ray.z*kaugus);
		//}
	}
	else if (player.removeBlockEvent())
	{
		std::cout << "Removing a block" << std::endl;	// remove this when removing/adding works

		//std::cout << std::fixed << ray.x << " " << ray.y << " " << ray.z << std::endl;
		// not sure how ray tracing works but I use small steps to increment the direction vector; probably not accurate

		double step = 0.1;

		auto trace = glm::vec3(0.0, 0.0, 0.0);

		for (double c = 1; glm::length(trace) <= maxDistance; c += step)
		{
			trace = glm::vec3(ray.x * c, ray.y * c, ray.z * c);
			auto tracePosition = playerPosition + trace;

			auto chunk = worldCoordToChunk(tracePosition);

			if (chunk != nullptr)
			{
				int x = static_cast<int>(tracePosition.x) % 16;
				int y = static_cast<int>(tracePosition.y) % 256;
				int z = static_cast<int>(tracePosition.z) % 16;

				if ((*chunk)->getBlock(x, y, z) != Block::Air)
				{
					(*chunk)->setBlock(Block::Air, x, y, z);
					std::cout << x << " " << y << " " << z << std::endl;
					break;
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
