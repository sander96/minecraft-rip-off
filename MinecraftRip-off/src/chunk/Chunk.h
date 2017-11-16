#ifndef CHUNK_H
#define CHUNK_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include "../Block/Block.h"
#include "../utilities/Shader_util.h"

class Chunk
{
public:
	Chunk();

	void setBlock(Block block, unsigned int x, unsigned int y, unsigned int z);

	//Block getBlock(unsigned int x, unsigned int y, unsigned int z) {return }

	void updateMesh();

	void render(shader_prog& shader);

private:
	std::tuple<int, int, int> getCoords(int index);

	int xPosition;
	int yPosition;

	bool requiresUpdate = true;

	std::array<Block, 16 * 16 * 256> blocks;
	std::vector<std::pair<GLuint, int>> temporaryBlocks;	// temp, very unefficient
};

#endif
