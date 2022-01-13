#pragma once

#include "Include.h"
#include "Input.h"
#include "Player.h"
#include "Camera.h"

namespace InputHandler {
	void update(double delta_time);
	void setPlayer(Player* player);
}