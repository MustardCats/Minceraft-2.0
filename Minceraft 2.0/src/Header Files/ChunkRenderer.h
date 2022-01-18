#pragma once

#include "Include.h"
#include "ChunkMesh.h"

namespace ChunkRenderer {
	void update();
	void render();
	void removeMesh(glm::ivec3 pos);
	void createMesh(Chunk* chunk);
	void regenMesh(glm::ivec3 pos);
}
