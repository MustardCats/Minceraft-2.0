#pragma once

#include "Include.h"
#include "InputHandler.h"
#include "Assets/BlockTypes.h"
#include "Entity/Player.h"
#include "Render/Camera.h"
#include "Render/ChunkRenderer.h"
#include "Render/Renderer.h"
#include "Render/UIRenderer.h"
#include "Window/Input.h"
#include "Window/Window.h"
#include "World/ChunkManager.h"
#include "World/ChunkWorker.h"

namespace Game {
	bool init();
	bool loadResources();
	bool gameLoop(double delta_time);
}