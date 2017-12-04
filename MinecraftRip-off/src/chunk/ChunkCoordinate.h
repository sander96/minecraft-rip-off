#ifndef CHUNKCOORDINATE_H
#define CHUNKCOORDINATE_H

#include <glm/glm.hpp>

class ChunkCoordinate
{
public:
	ChunkCoordinate(glm::vec3 position);

	unsigned int distance(ChunkCoordinate chunkCoordinate);

	friend bool operator==(ChunkCoordinate lhs, ChunkCoordinate rhs);
	friend bool operator!=(ChunkCoordinate lhs, ChunkCoordinate rhs);
	friend bool operator<(ChunkCoordinate lhs, ChunkCoordinate rhs);

	int getX() { return x; }
	int getZ() { return z; }

private:
	int x;
	int z;
};

#endif