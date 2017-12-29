#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <array>
#include <memory>
#include <vector>
#include "../block/Block.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ChunkMesh
{
public:
	ChunkMesh(int x, int z);
	~ChunkMesh();

	ChunkMesh(const ChunkMesh& chunkMesh) = delete;
	ChunkMesh& operator=(const ChunkMesh& chunkMesh) = delete;

	ChunkMesh(ChunkMesh&& chunkMesh);
	ChunkMesh& operator=(ChunkMesh&& chunkMesh);

	void generateChunkMesh(std::array<Block, 16 * 16 * 256>& blocks);
	void updateChunkMesh(bool& renderLocked);

	void render();

private:
	GLuint vertexArrayHandle;
	GLuint arrayBufferHandle;
	GLuint indicesHandle;
	GLsizei indicesCount;

	std::unique_ptr<std::vector<GLfloat>> vertexData;
	std::unique_ptr<std::vector<GLuint>> indices;

	int x;
	int z;
};

#endif