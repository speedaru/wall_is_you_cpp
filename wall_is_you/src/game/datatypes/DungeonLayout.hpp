#pragma once
#include "pch.h"


struct DungeonRoom {
	enum class Type { // type of room with which doors are open
		SOLID,
		SINGLE,
		DOUBLE_ADJACENT,
		DOUBLE_OPPOSITE,
		TRIPLE,
		QUAD
	} type;
	uint32_t rotations{ 0 };
};

struct DungeonRoomPos {
	uint8_t row{};
	uint8_t col{};
};

// row major
struct DungeonLayout {
    uint8_t width;
    uint8_t height;
    std::vector<DungeonRoom> tiles; // Flat vector representing the grid (index = y * width + x)
};
