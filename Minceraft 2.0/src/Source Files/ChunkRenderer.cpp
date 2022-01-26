#include "../Header Files/ChunkRenderer.h"

namespace ChunkRenderer {
	
	static std::vector<ChunkMesh*> chunk_meshes;
	static std::vector<ChunkMesh*> generating_chunk_meshes;

	void update() {
		static int frame = 0;
		int num_rows = c_height;
		if (generating_chunk_meshes.size() > 0 && frame == 0) {
			if (generating_chunk_meshes.at(generating_chunk_meshes.size() - 1)->generate(num_rows)) {
				chunk_meshes.push_back(generating_chunk_meshes.at(generating_chunk_meshes.size() - 1));
				generating_chunk_meshes.pop_back();
			}
		}
		frame++;
		if (frame == 2)
			frame = 0;
	}

	void render() {
		for (auto chunk_mesh : chunk_meshes) {
			if (chunk_mesh->isGenerated()) {
				chunk_mesh->render();
			}
		}
	}

	void removeMesh(glm::ivec3 pos) {
		for (int i = 0; i < chunk_meshes.size(); i++) {
			if (chunk_meshes.at(i)->pos() == pos) {
				delete chunk_meshes.at(i);
				chunk_meshes.erase(chunk_meshes.begin() + i);
				return;
			}
		}
		for (int i = 0; i < generating_chunk_meshes.size(); i++) {
			if (generating_chunk_meshes.at(i)->pos() == pos) {
				delete generating_chunk_meshes.at(i);
				generating_chunk_meshes.erase(generating_chunk_meshes.begin() + i);
				return;
			}
		}
	}

	void createMesh(Chunk* chunk) {
		generating_chunk_meshes.push_back(new ChunkMesh(chunk));
		//return;
		regenMesh(glm::ivec3(chunk->x - 1, chunk->y, chunk->z));
		regenMesh(glm::ivec3(chunk->x + 1, chunk->y, chunk->z));
		regenMesh(glm::ivec3(chunk->x, chunk->y - 1, chunk->z));
		regenMesh(glm::ivec3(chunk->x, chunk->y + 1, chunk->z));
		regenMesh(glm::ivec3(chunk->x, chunk->y, chunk->z - 1));
		regenMesh(glm::ivec3(chunk->x, chunk->y, chunk->z + 1));
	}

	void regenMesh(glm::ivec3 pos) {
		for (int i = 0; i < chunk_meshes.size(); i++) {
			if (chunk_meshes.at(i)->pos() == pos) {
				chunk_meshes.at(i)->clear();
				generating_chunk_meshes.push_back(chunk_meshes.at(i));
				chunk_meshes.erase(chunk_meshes.begin() + i);
				return;
			}
		}
		if (generating_chunk_meshes.size() > 0) {
			if (generating_chunk_meshes.at(generating_chunk_meshes.size() - 1)->pos() == pos) {
				generating_chunk_meshes.at(generating_chunk_meshes.size() - 1)->clear();
			}
		}
	}
}