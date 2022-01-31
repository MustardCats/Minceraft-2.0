#pragma once

#include "Include.h"

constexpr short c_length = 16; // x
constexpr short c_height = 256; // y
constexpr short c_width = 16; // z

struct Block {
	short id = 0;
	short sun_light = 0;
	short other_light = 0;
};

struct Chunk {
	Chunk() {
		x = y = z = 0;
	}

	Chunk(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	bool operator==(Chunk* other) {
		if (other->x == x &&
			other->y == y &&
			other->z == z)
			return true;
		return false;
	}

	bool operator==(glm::ivec3 other) {
		if (other.x == x &&
			other.y == y &&
			other.z == z)
			return true;
		return false;
	}

	Block* at(int x, int y, int z) {
		return &blocks[x + (y * c_length) + (z * c_length * c_height)];
	}

	int x, y, z;
	Block blocks[c_length * c_height * c_width];
};

namespace ChunkManager {
	bool init();
	void update();
	void makeLimitedChunkNear(glm::vec3 pos);
	void makeChunkNear(glm::vec3 pos);
	Chunk* getNewChunk();
	glm::ivec3 posToChunk(glm::ivec3 pos);
	glm::ivec3 getDeleteChunk();
	bool containsDeleteChunks();
	void deleteChunks(glm::vec3 pos);
	Chunk* getChunk(int x, int y, int z);
}