#pragma once

#include "../Include.h"
#include "Chunk.h"
#include "../Render/ChunkMeshBuilder.h"

// https://github.com/Hopson97/MineCraft-One-Week-Challenge/blob/master/Source/Maths/Vector2XZ.h

namespace ChunkManager {
	bool init();
	void update();
	glm::ivec3 posToChunk(glm::ivec3 pos);
	glm::ivec3 chunkRelative(glm::ivec3 pos);
	Chunk* getChunk(glm::ivec3 pos);
	Block* getBlock(glm::ivec3 pos);
	bool chunkExists(glm::ivec3 pos);
	std::mutex* chunkLock();
	bool addChunk(Chunk* chunk);
	void deleteEmptyChunks();
	std::queue<glm::ivec3> findEmptyChunks(glm::ivec3 chunk_pos);
	robin_hood::unordered_map<glm::ivec3, Chunk*>* getChunks();
	void serialize(Chunk* chunk);
	bool deserialize(Chunk* chunk);
	void generateChunk(Chunk* chunk);
	void deleteChunk(glm::ivec3 pos);
	void changeBlock(glm::ivec3 pos, int id);
	void flushChunks();
	bool deletingChunks();
}