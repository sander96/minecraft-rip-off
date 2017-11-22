#include "ChunkCoordinate.h"

ChunkCoordinate::ChunkCoordinate(glm::vec3 position)
{
	x = std::floor(position[0] / 16);
	z = std::floor(position[2] / 16);
}

unsigned int ChunkCoordinate::distance(ChunkCoordinate chunkCoordinate)
{
	int a = std::abs(x - chunkCoordinate.x);
	int b = std::abs(z - chunkCoordinate.z);
	return std::sqrt(a * a + b * b);
}

bool operator==(ChunkCoordinate lhs, ChunkCoordinate rhs)
{
	return lhs.x == rhs.x && lhs.z == rhs.z;
}

bool operator!=(ChunkCoordinate lhs, ChunkCoordinate rhs)
{
	return !(lhs == rhs);
}

bool operator<(ChunkCoordinate lhs, ChunkCoordinate rhs)
{
	return lhs.distanceFromMiddle < rhs.distanceFromMiddle;
}