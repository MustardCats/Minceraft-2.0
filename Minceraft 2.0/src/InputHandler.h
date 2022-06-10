#pragma once

#include "Include.h"
#include "Entity/Player.h"
#include "Render/Camera.h"
#include "Render/ChunkRenderer.h"
#include "Render/DebugRenderer.h"
#include "Window/Input.h"
#include "World/ChunkManager.h"
#include "World/ChunkWorker.h"

namespace InputHandler {
	void update(double delta_time);
	void setPlayer(Player* player);
}