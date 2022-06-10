#pragma once

#include "../Include.h"
#include "../Entity/Player.h"
#include "../Render/Mesh/LineMesh.h"

namespace DebugRenderer {
	void render();
	void toggleRigidBodies();
	void setPlayer(Player* player);
}
