#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>

enum class Block : uint8_t
{
	Air, Dirt, Sand, Grass, Stone, Wood, Leaves, Cactus, Water, Cloud,
	NULL_BLOCK = UINT8_MAX	// only used for saving and loading chunks
};

#endif
