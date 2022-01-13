#pragma once

#include "../Header Files/Include.h"

constexpr short c_length = 16; // x
constexpr short c_height = 16; // y
constexpr short c_width = 16; // z

struct Block {
	int id = 0;
	short sun_light = 0;
	short other_light = 0;
};

struct Chunk {
	int x, y, z;
	Block blocks[c_length * c_height * c_width];
};

namespace ChunkManager {
	bool init();
	void update();

}