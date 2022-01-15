#pragma once

#include "Include.h"
#include "Shader.h"
#include "Mesh.h"
#include "Window.h"
#include "Camera.h"
#include "BlockTypes.h"
#include "ChunkMesh.h"
#include "ChunkManager.h"

namespace Renderer {
	bool init();
	void render();
	void createChunkMesh(Chunk* chunk);
	void removeChunkMesh(glm::ivec3 pos);
}