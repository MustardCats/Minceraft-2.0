#pragma once

#include "../Include.h"
#include "../Constants.h"
#include "../World/Chunk.h"

namespace ChunkMeshBuilder {
	void setChunks(robin_hood::unordered_map<glm::ivec3, Chunk*>* chunks);
	Chunk* findNewChunk();
	void buildChunkMesh(Chunk* chunk);
}