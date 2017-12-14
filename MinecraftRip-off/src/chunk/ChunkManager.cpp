#include "ChunkManager.h"
#include "../Block/Cube.h"
#include <stdlib.h>
#include "../entities/Picker.h"

ChunkManager::ChunkManager()
	:
	previousPlayerPosition{ glm::vec3(123.0, 0.0, 0.0) },	// to force chunk update
	radius{ settings.getChunkRadius() },
	biome{ PerlinNoise::PerlinNoise(100) }
{
	updateChunks(glm::vec3(0.0, 0.0, 0.0),0,glm::vec3(0,0,0));
}

void ChunkManager::updateChunks(glm::vec3 playerPosition,int seis,glm::vec3 ray)
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
	//seis == 1 kutsub v�lja bloki lisamise
	// veel on buggine, kuna funktsioon ei oska teise chunki lisada ja selle asemele alusatab teiselt poolt
	//ntks kui lisad blocki olles ise 0,0,0, siis m�ned l�hevad neist 16,0,0 ja 0,0,16..
	if (seis == 1) {
		auto& currChunk = chunks.find(playerChunkCoord);
		//kui kaugule saame blokke panna
		int kaugus = 4;
		//et ei paneks lambist �hku blokki, siis kontroll, et temast tagapool ikka on blokk..
			std::cout << ray.x << "," << ray.y << "," << ray.z << std::endl;
			int temp = 0;
			bool found = false;
			//kas on kuni kauguseni vaba(et ei paneks teiste blokkide taha, vaid ette)
			for (int i = 0;i <= kaugus;i++) {
				if (currChunk->second->getBlock(playerPosition.x + ray.x*i, playerPosition.y + ray.y*i, playerPosition.z + ray.z*i) != Block::Air) {
					temp = i - 1;
					found = true;
				}
			}
			if (found == true) {
				kaugus = temp;
			}
			//selleks,et lampi �hku ei saaks blokke riputada..
			if (currChunk->second->getBlock(playerPosition.x + ray.x*(kaugus + 1), playerPosition.y + ray.y*(kaugus + 1), playerPosition.z + ray.z*(kaugus + 1)) != Block::Air) {
				currChunk->second->setBlock(Block::Stone, playerPosition.x + ray.x*kaugus, playerPosition.y + ray.y*kaugus, playerPosition.z + ray.z*kaugus);
			}
			seis = 0;
	}
	//seis == -1 on eemaldamiseks.
	//miski p�rast kustutab ainult neid blokke, mille oleme ise lisanud..
	if (seis == -1) {
		auto& currChunk = chunks.find(playerChunkCoord);
		//kui kaugule saame blokke panna
		int kaugus = 4;
		std::cout << ray.x << "," << ray.y << "," << ray.z << std::endl;//enda jaoks
		int temp = 0;
		bool found = false;
		//kas on kuni kauguseni vaba(et kustutaks ainult seda, mis on k�ige ees)
		for (int i = 0;i <= kaugus;i++) {
			if (currChunk->second->getBlock(playerPosition.x + ray.x*i, playerPosition.y + ray.y*i, playerPosition.z + ray.z*i) != Block::Air) {
				temp = i;
				found = true;
			}
		}
		if (found == true) {
			kaugus = temp;
		}
		currChunk->second->setBlock(Block::Air, playerPosition.x + ray.x*kaugus, playerPosition.y + ray.y*kaugus, playerPosition.z + ray.z*kaugus);
		seis = 0;
	}
}

std::unique_ptr<Chunk> ChunkManager::createChunk(ChunkCoordinate coordinate)
{
	std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(coordinate.getX() * 16, coordinate.getZ() * 16);
	std::cout << "Chunk" << coordinate.getX() << ";" << coordinate.getZ() << std::endl;
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
					if (x == 2 && z == 3)
					{
						int size = 5;
						int y = y1 + y2 + y3;
						for (int i = 0; i < size;i++) {
							chunk->setBlock(Block::Cactus, x, y + i, z);
						}
					}
					else {
						chunk->setBlock(Block::Sand, x, y1 + y2 + y3, z);
					}
				}
				else
				{
					if (x == 2 && z == 3 || x == 8 && z == 6)
					{
						//SIZE peab olema kuidagi seotud noisega, siis suvalise suurusega puud..
						int size = floor(y1);
						int y = y1 + y2 + y3;
						//t�vi
						for (int i = 0; i < size;i++) {
							chunk->setBlock(Block::Wood, x, y + i, z);
						}
						//lehed �mber?
						int muutuja = 2;
						if (size > 5) {
							muutuja = 3;
						}
						for (int i = 0;i < size / 2 + 1;i++) {
							if (i == size / 2 - 1) {
								muutuja = 2;
							}
							if (i == size / 2) {
								for (int p = -muutuja + 1;p < muutuja;p++)
								{
									chunk->setBlock(Block::Leaves, x, y + size - 1 + i, z + p);
									chunk->setBlock(Block::Leaves, x + p, y + size - 1 + i, z);
								}
							}
							else {
								for (int j = -muutuja + 1;j < muutuja;j++) {
									for (int p = -muutuja + 1;p < muutuja;p++) {
										if (j != 0 || p != 0 || i > 0) {
											chunk->setBlock(Block::Leaves, x + j, y + size - 1 + i, z + p);

										}
									}
								}
								if (muutuja == 3 && i == size / 2 - 2) {
									chunk->setBlock(Block::Air, x + muutuja - 1, y + size - 1 + i, z + muutuja - 1);
									chunk->setBlock(Block::Air, x - muutuja + 1, y + size - 1 + i, z + muutuja - 1);
									chunk->setBlock(Block::Air, x + muutuja - 1, y + size - 1 + i, z - muutuja + 1);
									chunk->setBlock(Block::Air, x - muutuja + 1, y + size - 1 + i, z - muutuja + 1);
								}
							}
						}
						chunk->setBlock(Block::Leaves, x, y + size + size / 2, z);
					}
					else
					{
						chunk->setBlock(Block::Grass, x, y1 + y2 + y3, z);
					}
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
