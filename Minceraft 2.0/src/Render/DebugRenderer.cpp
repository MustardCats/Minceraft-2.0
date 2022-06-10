#include "DebugRenderer.h"

namespace DebugRenderer {

	static Player* player = nullptr;
	static bool rigit_bodies = false;

	void renderRigidBodies() {
		if (player != nullptr) {
			LineMesh mesh;
			mesh.addRect(Rectangle(player->getPos(), player->getSize()), glm::vec3(1.0f));
			mesh.generateBuffers();
			mesh.render();
		}
	}

	void render() {
		if (rigit_bodies)
			renderRigidBodies();
	}

	void toggleRigidBodies(){
		if (rigit_bodies) {
			rigit_bodies = false;
		}
		else {
			rigit_bodies = true;
		}
	}

	void setPlayer(Player* player) {
		DebugRenderer::player = player;
	}
}