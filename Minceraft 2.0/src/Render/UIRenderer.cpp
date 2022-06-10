#include "UIRenderer.h"

namespace UIRenderer {

	static Player* player = nullptr;
	static glm::ivec2 window_size = glm::ivec2(0);
	static BasicMesh inv_mesh;

	void recreateMeshes() {
		inv_mesh.clear();
		glm::ivec2 size = glm::ivec2(75);
		glm::ivec2 offset = glm::ivec2(Window::getSize() / 2 - glm::ivec2(size.x * 4.5f, size.y * 2.0f));
		int padding = 8;
		std::array<glm::vec2, 2> slot_uvs = BlockTypes::getFaceUVs("slot");
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 9; x++) {
				inv_mesh.addRect(offset + glm::ivec2(size.x * x, size.y * y), size, slot_uvs);
				Item item = player->getInventory()->getItems()->at((y * 9) + x);
				if (item.id > 0) {
					std::array<std::array<glm::vec2, 2>, 6> block_uvs = BlockTypes::getBlockUVs(item.id);
					inv_mesh.addRect(offset + glm::ivec2(size.x * x, size.y * y) + padding, size - (padding * 2), block_uvs[0]);
				}
			}
		}
		inv_mesh.generateBuffers();
	}

	void renderInventory() {
		if (player->isInvOpen()) {
			inv_mesh.render();
		}

	}

	void render() {
		if (player != nullptr) {
			if (Window::getSize() != window_size) {
				window_size = Window::getSize();
				recreateMeshes();
			}
			// TODO: change to only recreate meshes when items are changed
			else {
				recreateMeshes();
			}
			renderInventory();
		}
	}

	void setPlayer(Player* player) {
		UIRenderer::player = player;
	}
}