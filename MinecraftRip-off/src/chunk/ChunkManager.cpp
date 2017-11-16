#include "ChunkManager.h"
#include "../Block/Cube.h"

ChunkManager::ChunkManager()
{
	std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(Chunk{});

	for (int x = 0; x < 16; ++x)		// temp
	{
		for (int z = 0; z < 16; ++z)
		{
			for (int y = 0; y < 20; ++y)
			{
				if (y == 19)
				{
					chunk->setBlock(Block::Grass, x, y, z);
				}
				else if (y > 15)
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

	chunks[std::pair<int, int>(0, 0)] = std::move(chunk);		// currently creates only one chunk
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
