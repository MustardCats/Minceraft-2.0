#pragma once

#include "../Include.h"
#include "../Render/Mesh/ChunkMesh.h"
#include "../World/Chunk.h"

namespace ChunkRenderer {
	void setChunks(robin_hood::unordered_map<glm::ivec3, Chunk*>* chunks);
	void render();
	bool rendering();
}