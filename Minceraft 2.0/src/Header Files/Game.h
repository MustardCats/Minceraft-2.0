#pragma once

#include "Include.h"
#include "Window.h"
#include "Input.h"
#include "Renderer.h"
#include "Camera.h"
#include "InputHandler.h"
#include "Player.h"
#include "BlockTypes.h"

namespace Game {
	bool init();
	bool loadResources();
	bool gameLoop(double delta_time);
}