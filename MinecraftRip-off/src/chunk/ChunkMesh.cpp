#include "ChunkMesh.h"
#include <iostream>
#include "../utilities/ResourceManager.h"

namespace
{
	GLuint addVertex(std::vector<GLfloat>& vertexData, std::initializer_list<float> list)
	{
		auto position = std::search(vertexData.begin(), vertexData.end(), list.begin(), list.end());

		while (position != vertexData.end())
		{
			int distance = std::distance(vertexData.begin(), position);

			if (distance % list.size() == 0)
			{
				return distance;
			}

			position = std::search(std::next(position), vertexData.end(), list.begin(), list.end());
		}

		vertexData.insert(vertexData.end(), list);
		return vertexData.size() / list.size() - 1;
	}

	bool rightFaceCovered(std::array<Block, 16 * 16 * 256>& blocks, int index)
	{
		if (index % 16 == 15)
		{
			return false;
		}

		if (blocks[index + 1] == Block::Air)
		{
			return false;
		}

		return true;
	}

	bool leftFaceCovered(std::array<Block, 16 * 16 * 256>& blocks, int index)
	{
		if (index % 16 == 0)
		{
			return false;
		}

		if (blocks[index - 1] == Block::Air)
		{
			return false;
		}

		return true;
	}

	bool frontFaceCovered(std::array<Block, 16 * 16 * 256>& blocks, int index)
	{
		if ((index % 256) / 16 == 15)
		{
			return false;
		}

		if (blocks[index + 16] == Block::Air)
		{
			return false;
		}

		return true;
	}

	bool backFaceCovered(std::array<Block, 16 * 16 * 256>& blocks, int index)
	{
		if ((index % 256) / 16 == 0)
		{
			return false;
		}

		if (blocks[index - 16] == Block::Air)
		{
			return false;
		}

		return true;
	}

	bool topFaceCovered(std::array<Block, 16 * 16 * 256>& blocks, int index)
	{
		if (index / 256 == 255)
		{
			return false;
		}

		if (blocks[index + 256] == Block::Air)
		{
			return false;
		}

		return true;
	}

	bool bottomFaceCovered(std::array<Block, 16 * 16 * 256>& blocks, int index)
	{
		if (index / 256 == 0)
		{
			return false;
		}

		if (blocks[index - 256] == Block::Air)
		{
			return false;
		}

		return true;
	}

	enum class Face
	{
		Top, Side, Bottom
	};

	std::pair<glm::vec2, glm::vec2> calculateUV(Block block, Face face)
	{
		std::pair<glm::vec2, glm::vec2> uv;

		if (block == Block::Grass)
		{
			uv.first = glm::vec2(0.0, 256.0);
			uv.second = glm::vec2(16.0, 240.0);
		}
		else if (block == Block::Dirt)
		{
			uv.first = glm::vec2(16.0, 256.0);
			uv.second = glm::vec2(32.0, 240.0);
		}
		else if (block == Block::Sand)
		{
			uv.first = glm::vec2(32.0, 256.0);
			uv.second = glm::vec2(48.0, 240.0);
		}
		else if (block == Block::Stone)
		{
			uv.first = glm::vec2(48.0, 256.0);
			uv.second = glm::vec2(64.0, 240.0);
		}
		else if (block == Block::Wood)
		{
			uv.first = glm::vec2(64.0, 256.0);
			uv.second = glm::vec2(80.0, 240.0);
		}
		else if (block == Block::Leaves)
		{
			uv.first = glm::vec2(80.0, 256.0);
			uv.second = glm::vec2(96.0, 240.0);
		}

		if (face == Face::Side)
		{
			uv.first[1] -= 16;
			uv.second[1] -= 16;
		}
		else if (face == Face::Bottom)
		{
			uv.first[1] -= 32;
			uv.second[1] -= 32;
		}

		float size = 256.0;

		uv.first[0] /= size;
		uv.first[1] /= size;

		uv.second[0] /= size;
		uv.second[1] /= size;

		return uv;
	}
}

ChunkMesh::ChunkMesh(int x, int z)
	:
	x{ x },
	z{ z }
{

}

ChunkMesh::~ChunkMesh()
{
	glDeleteVertexArrays(1, &vertexArrayHandle);
	glDeleteBuffers(1, &arrayBufferHandle);
	glDeleteBuffers(1, &indicesHandle);
}

ChunkMesh::ChunkMesh(ChunkMesh&& chunkMesh)
{
	x = chunkMesh.x;
	z = chunkMesh.z;

	vertexArrayHandle = chunkMesh.vertexArrayHandle;
	chunkMesh.vertexArrayHandle = 0;

	arrayBufferHandle = chunkMesh.arrayBufferHandle;
	chunkMesh.arrayBufferHandle = 0;

	indicesHandle = chunkMesh.indicesHandle;
	chunkMesh.indicesHandle = 0;

	indicesCount = chunkMesh.indicesCount;
	chunkMesh.indicesCount = 0;
}

ChunkMesh& ChunkMesh::operator=(ChunkMesh&& chunkMesh)
{
	x = chunkMesh.x;
	z = chunkMesh.z;

	vertexArrayHandle = chunkMesh.vertexArrayHandle;
	chunkMesh.vertexArrayHandle = 0;

	arrayBufferHandle = chunkMesh.arrayBufferHandle;
	chunkMesh.arrayBufferHandle = 0;

	indicesHandle = chunkMesh.indicesHandle;
	chunkMesh.indicesHandle = 0;

	indicesCount = chunkMesh.indicesCount;
	chunkMesh.indicesCount = 0;

	return *this;
}

void ChunkMesh::updateChunkMesh(std::array<Block, 16 * 16 * 256>& blocks)
{
	std::vector<GLfloat> vertexData;
	std::vector<GLuint> indices;

	int x = 0;
	int y = 0;
	int z = 0;

	for (int i = 0; i < blocks.size(); ++i, x += 2)
	{
		if (x == 2 * 16)	// block width is 2
		{
			x = 0;
			z += 2;

			if (z == 2 * 16)
			{
				z = 0;
				y += 2;
			}
		}

		if (blocks[i] == Block::Air)
		{
			continue;
		}

		if (!rightFaceCovered(blocks, i)) // X Y Z U V NORMAL
		{
			auto uv = calculateUV(blocks[i], Face::Side);

			GLuint bottomLeft = addVertex(vertexData, { x + 1.0f, y + -1.0f, z + 1.0f, uv.first[0], uv.second[1], 1.0f, 0.0f, 0.0f });		// bottom left
			GLuint bottomRight = addVertex(vertexData, { x + 1.0f, y + -1.0f, z + -1.0f, uv.second[0], uv.second[1], 1.0f, 0.0f, 0.0f });	// bottom right
			GLuint topRight = addVertex(vertexData, { x + 1.0f, y + 1.0f, z + -1.0f, uv.second[0], uv.first[1], 1.0f, 0.0f, 0.0f });		// top right
			GLuint topLeft = addVertex(vertexData, { x + 1.0f, y + 1.0f, z + 1.0f, uv.first[0], uv.first[1], 1.0f, 0.0f, 0.0f });			// top left

			indices.insert(indices.end(), { bottomLeft, bottomRight, topRight });
			indices.insert(indices.end(), { bottomLeft, topRight, topLeft });
		}

		if (!leftFaceCovered(blocks, i))
		{
			auto uv = calculateUV(blocks[i], Face::Side);

			GLuint bottomLeft = addVertex(vertexData, { x + -1.0f, y + -1.0f, z + -1.0f, uv.first[0], uv.second[1], -1.0f, 0.0f, 0.0f });	// bottom left
			GLuint bottomRight = addVertex(vertexData, { x + -1.0f, y + -1.0f, z + 1.0f, uv.second[0], uv.second[1], -1.0f, 0.0f, 0.0f });	// bottom right
			GLuint topRight = addVertex(vertexData, { x + -1.0f, y + 1.0f, z + 1.0f, uv.second[0], uv.first[1], -1.0f, 0.0f, 0.0f });		// top right
			GLuint topLeft = addVertex(vertexData, { x + -1.0f, y + 1.0f, z + -1.0f, uv.first[0], uv.first[1], -1.0f, 0.0f, 0.0f });		// top left

			indices.insert(indices.end(), { bottomLeft, bottomRight, topRight });
			indices.insert(indices.end(), { bottomLeft, topRight, topLeft });
		}

		if (!frontFaceCovered(blocks, i))
		{
			auto uv = calculateUV(blocks[i], Face::Side);

			GLuint bottomLeft = addVertex(vertexData, { x + -1.0f, y + -1.0f, z + 1.0f, uv.first[0], uv.second[1], 0.0f, 0.0f, 1.0f });		// bottom left
			GLuint bottomRight = addVertex(vertexData, { x + 1.0f, y + -1.0f, z + 1.0f, uv.second[0], uv.second[1], 0.0f, 0.0f, 1.0f });	// bottom right
			GLuint topRight = addVertex(vertexData, { x + 1.0f, y + 1.0f, z + 1.0f, uv.second[0], uv.first[1], 0.0f, 0.0f, 1.0f });			// top right
			GLuint topLeft = addVertex(vertexData, { x + -1.0f, y + 1.0f, z + 1.0f, uv.first[0], uv.first[1], 0.0f, 0.0f, 1.0f });			// top left

			indices.insert(indices.end(), { bottomLeft, bottomRight, topRight });
			indices.insert(indices.end(), { bottomLeft, topRight, topLeft });
		}

		if (!backFaceCovered(blocks, i))
		{
			auto uv = calculateUV(blocks[i], Face::Side);

			GLuint bottomLeft = addVertex(vertexData, { x + 1.0f, y + -1.0f, z + -1.0f, uv.first[0], uv.second[1], 0.0f, 0.0f, -1.0f });	// bottom left
			GLuint bottomRight = addVertex(vertexData, { x + -1.0f, y + -1.0f, z + -1.0f, uv.second[0], uv.second[1], 0.0f, 0.0f, -1.0f });	// bottom right
			GLuint topRight = addVertex(vertexData, { x + -1.0f, y + 1.0f, z + -1.0f, uv.second[0], uv.first[1], 0.0f, 0.0f, -1.0f });		// top right
			GLuint topLeft = addVertex(vertexData, { x + 1.0f, y + 1.0f, z + -1.0f, uv.first[0], uv.first[1], 0.0f, 0.0f, -1.0f });			// top left

			indices.insert(indices.end(), { bottomLeft, bottomRight, topRight });
			indices.insert(indices.end(), { bottomLeft, topRight, topLeft });
		}

		if (!topFaceCovered(blocks, i))
		{
			auto uv = calculateUV(blocks[i], Face::Top);

			GLuint bottomLeft = addVertex(vertexData, { x + -1.0f, y + 1.0f, z + 1.0f, uv.first[0], uv.second[1], 0.0f, 1.0f, 0.0f });		// bottom left
			GLuint bottomRight = addVertex(vertexData, { x + 1.0f, y + 1.0f, z + 1.0f, uv.second[0], uv.second[1], 0.0f, 1.0f, 0.0f });		// bottom right
			GLuint topRight = addVertex(vertexData, { x + 1.0f, y + 1.0f, z + -1.0f, uv.second[0], uv.first[1], 0.0f, 1.0f, 0.0f });		// top right
			GLuint topLeft = addVertex(vertexData, { x + -1.0f, y + 1.0f, z + -1.0f, uv.first[0], uv.first[1], 0.0f, 1.0f, 0.0f });			// top left

			indices.insert(indices.end(), { bottomLeft, bottomRight, topRight });
			indices.insert(indices.end(), { bottomLeft, topRight, topLeft });
		}

		if (!bottomFaceCovered(blocks, i))
		{
			auto uv = calculateUV(blocks[i], Face::Bottom);

			GLuint bottomLeft = addVertex(vertexData, { x + -1.0f, y + -1.0f, z + -1.0f, uv.first[0], uv.second[1], 0.0f, -1.0f, 0.0f });		// bottom left
			GLuint bottomRight = addVertex(vertexData, { x + 1.0f, y + -1.0f, z + -1.0f, uv.second[0], uv.second[1], 0.0f, -1.0f, 0.0f });		// bottom right
			GLuint topRight = addVertex(vertexData, { x + 1.0f, y + -1.0f, z + 1.0f, uv.second[0], uv.first[1], 0.0f, -1.0f, 0.0f });		// top right
			GLuint topLeft = addVertex(vertexData, { x + -1.0f, y + -1.0f, z + 1.0f, uv.first[0], uv.first[1], 0.0f, -1.0f, 0.0f });		// top left

			indices.insert(indices.end(), { bottomLeft, bottomRight, topRight });
			indices.insert(indices.end(), { bottomLeft, topRight, topLeft });
		}
	}

	//std::cout << "Vertices count: " << vertexData.size() / 8 << std::endl;
	//std::cout << "Triangle count: " << indices.size() / 3 << std::endl;

	indicesCount = indices.size();

	glGenBuffers(1, &arrayBufferHandle);
	glGenVertexArrays(1, &vertexArrayHandle);

	glBindVertexArray(vertexArrayHandle);

	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indicesHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

	shader_prog& shader = ResourceManager::getInstance().getShaderHandle(Shader::Texture);

	GLuint loc = glGetAttribLocation(shader.getProg(), "position");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(0 * sizeof(float)));

	loc = glGetAttribLocation(shader.getProg(), "uv");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));

	loc = glGetAttribLocation(shader.getProg(), "normal");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(5 * sizeof(float)));

	glBindVertexArray(0);
}

void ChunkMesh::render()
{
	glm::mat4 matrix{ 1.0 };
	matrix = glm::scale(matrix, glm::vec3(0.5));
	matrix = glm::translate(matrix, glm::vec3(2 * x, 2.0, 2 * z));

	shader_prog& shader = ResourceManager::getInstance().getShaderHandle(Shader::Texture);
	shader.uniformMatrix4fv("modelMatrix", matrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::getInstance().getTextureHandle(Texture::Atlas));

	glBindVertexArray(vertexArrayHandle);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
