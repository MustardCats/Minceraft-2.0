#include "ChunkRenderer.h"

namespace ChunkRenderer {

	static robin_hood::unordered_map<glm::ivec3, Chunk*>* chunks;
	static bool is_rendering = false;

	void setChunks(robin_hood::unordered_map<glm::ivec3, Chunk*>* chunks) {
		ChunkRenderer::chunks = chunks;
	}

	void render() {
		is_rendering = true;
		for (auto& chunk : *chunks) {
			if (chunk.second == nullptr) {
				std::cout << "FUCKFUCKFUCK " << chunk.first.x << " " << chunk.first.z << "\n";
				//assert(false);
				continue;
			}
			if (chunk.second->state == Chunk::State::BUFFERS) {
				chunk.second->mesh.render();
			}
		}
		is_rendering = false;
	}

	bool rendering() {
		return is_rendering;
	}
}