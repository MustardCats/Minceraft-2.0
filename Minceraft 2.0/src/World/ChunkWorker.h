#pragma once

#include "../Include.h"
#include "ChunkManager.h"
#include "../Constants.h"
#include "../Render/ChunkMeshBuilder.h"
#include "../Render/ChunkRenderer.h"

namespace ChunkWorker {
	void start();
	void end();
	bool isActive();
	Chunk* getNewChunk();
	void setPlayerPos(glm::vec3* player_pos);
	void togglePauseMode();
	void unpause();
}