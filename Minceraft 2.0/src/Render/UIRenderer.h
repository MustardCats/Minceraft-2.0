#pragma once

#include "../Include.h"
#include "Mesh/BasicMesh.h"
#include "../Assets/BlockTypes.h"
#include "../Entity/Player.h"
#include "../Window/Window.h"

namespace UIRenderer {
	void render();
	void setPlayer(Player* player);
}