#include "ChunkMeshBuilder.h"

namespace ChunkMeshBuilder {

	static robin_hood::unordered_map<glm::ivec3, Chunk*>* chunks;
	
	void setChunks(robin_hood::unordered_map<glm::ivec3, Chunk*>* chunks) {
		ChunkMeshBuilder::chunks = chunks;
	}

	Chunk* findNewChunk() {
		robin_hood::unordered_map<glm::ivec3, Chunk*>::iterator it;
		for (it = chunks->begin(); it != chunks->end(); it++) {
			if (!(*it).second->mesh.isGenerated()) {
				return (*it).second;
			}
		}
		return nullptr;
	}
	
	Chunk* findChunk(glm::ivec3 pos) {
		if (chunks->find(pos) != chunks->end()) {
			Chunk* chunk = (*chunks)[pos];
			// wait for main thread to finish
			if (chunk == nullptr || chunk->state == Chunk::State::DELETING) {
				return nullptr;
			}
			if (chunk->lock.try_lock()) {
				return chunk;
			}
		}
		return nullptr;
	}

	std::array<Chunk*, 4> findAdjacentChunks(Chunk* chunk) {
		std::array<Chunk*, 4> adj_chunks{ nullptr };
		if (chunk != nullptr) {
			adj_chunks[0] = findChunk(chunk->pos + glm::ivec3(-1, 0, 0));
			adj_chunks[1] = findChunk(chunk->pos + glm::ivec3(1, 0, 0));
			adj_chunks[2] = findChunk(chunk->pos + glm::ivec3(0, 0, -1));
			adj_chunks[3] = findChunk(chunk->pos + glm::ivec3(0, 0, 1));
		}
		return adj_chunks;
	}

	void buildChunkMesh(Chunk* chunk) {
		if (chunk == nullptr) {
			return;
		}
		chunk->state == Chunk::State::GENERATED;
		ChunkMesh* mesh = &chunk->mesh;
		std::array<Chunk*, 4> adj_chunks = findAdjacentChunks(chunk);
		mesh->clear();
		static const float side_light = 0.95f;
		static const float bottom_light = 0.90f;
		glm::vec3 chunk_pos = { chunk->pos.x * pc::c_length, chunk->pos.y * pc::c_height, chunk->pos.z * pc::c_width };
		bool full = true;
		bool prev_full = true;
		for (int y = 0; y < pc::c_height; y++) {
			prev_full = full;
			full = true;
			for (int x = 0; x < pc::c_length; x++) {
				for (int z = 0; z < pc::c_width; z++) {
					if (chunk->blocks[x][y][z].id != 0) {
						glm::vec3 pos = glm::vec3(chunk_pos.x + x, chunk_pos.y + y, chunk_pos.z + z);
						std::array<std::array<glm::vec2, 2>, 6> block_uvs = BlockTypes::getBlockUVs(chunk->blocks[x][y][z].id);
						// front
						if ((z > 0 && chunk->blocks[x][y][z - 1].id == 0) || (z == 0 && adj_chunks[2] != nullptr && adj_chunks[2]->blocks[x][y][pc::c_width - 1].id == 0)) {
							float front[] = {
								pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f, block_uvs[0][0].x, block_uvs[0][1].y, side_light, // bottom left
								pos.x + 0.5f, pos.y - 0.5f, pos.z - 0.5f, block_uvs[0][1].x, block_uvs[0][1].y, side_light, // bottom right
								pos.x + 0.5f, pos.y + 0.5f, pos.z - 0.5f, block_uvs[0][1].x, block_uvs[0][0].y, side_light, // top right
								pos.x - 0.5f, pos.y + 0.5f, pos.z - 0.5f, block_uvs[0][0].x, block_uvs[0][0].y, side_light  // top left
							};
							int num_vertices = mesh->vertices.size() / 6;
							int new_indices[] = {
								num_vertices, num_vertices + 1, num_vertices + 2,
								num_vertices, num_vertices + 2, num_vertices + 3
							};
							mesh->vertices.insert(mesh->vertices.begin(), std::begin(front), std::end(front));
							mesh->indices.insert(mesh->indices.begin(), std::begin(new_indices), std::end(new_indices));
						}
						// back
						if ((z < (pc::c_width - 1) && chunk->blocks[x][y][z + 1].id == 0) || (z == (pc::c_width - 1) && adj_chunks[3] != nullptr && adj_chunks[3]->blocks[x][y][0].id == 0)) {
							float back[] = {
								pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.5f, block_uvs[1][0].x, block_uvs[1][1].y, side_light, // bottom left
								pos.x - 0.5f, pos.y - 0.5f, pos.z + 0.5f, block_uvs[1][1].x, block_uvs[1][1].y, side_light, // bottom right
								pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.5f, block_uvs[1][1].x, block_uvs[1][0].y, side_light, // top right
								pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f, block_uvs[1][0].x, block_uvs[1][0].y, side_light  // top left
							};
							int num_vertices = mesh->vertices.size() / 6;
							int new_indices[] = {
								num_vertices, num_vertices + 1, num_vertices + 2,
								num_vertices, num_vertices + 2, num_vertices + 3
							};
							mesh->vertices.insert(mesh->vertices.begin(), std::begin(back), std::end(back));
							mesh->indices.insert(mesh->indices.begin(), std::begin(new_indices), std::end(new_indices));
						}
						// left
						if ((x > 0 && chunk->blocks[x - 1][y][z].id == 0) || (x == 0 && adj_chunks[0] != nullptr && adj_chunks[0]->blocks[pc::c_length - 1][y][z].id == 0)) {
							float left[] = {
								pos.x - 0.5f, pos.y - 0.5f, pos.z + 0.5f, block_uvs[2][0].x, block_uvs[2][1].y, side_light, // bottom left
								pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f, block_uvs[2][1].x, block_uvs[2][1].y, side_light, // bottom right
								pos.x - 0.5f, pos.y + 0.5f, pos.z - 0.5f, block_uvs[2][1].x, block_uvs[2][0].y, side_light, // top right
								pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.5f, block_uvs[2][0].x, block_uvs[2][0].y, side_light  // top left
							};
							int num_vertices = mesh->vertices.size() / 6;
							int new_indices[] = {
								num_vertices, num_vertices + 1, num_vertices + 2,
								num_vertices, num_vertices + 2, num_vertices + 3
							};
							mesh->vertices.insert(mesh->vertices.begin(), std::begin(left), std::end(left));
							mesh->indices.insert(mesh->indices.begin(), std::begin(new_indices), std::end(new_indices));
						}
						// right
						if ((x < (pc::c_length - 1) && chunk->blocks[x + 1][y][z].id == 0) || (x == (pc::c_length - 1) && adj_chunks[1] != nullptr && adj_chunks[1]->blocks[0][y][z].id == 0)) {
							float right[] = {
								pos.x + 0.5f, pos.y - 0.5f, pos.z - 0.5f, block_uvs[3][0].x, block_uvs[3][1].y, side_light, // bottom left
								pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.5f, block_uvs[3][1].x, block_uvs[3][1].y, side_light, // bottom right
								pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f, block_uvs[3][1].x, block_uvs[3][0].y, side_light, // top right
								pos.x + 0.5f, pos.y + 0.5f, pos.z - 0.5f, block_uvs[3][0].x, block_uvs[3][0].y, side_light // top left
							};
							int num_vertices = mesh->vertices.size() / 6;
							int new_indices[] = {
								num_vertices, num_vertices + 1, num_vertices + 2,
								num_vertices, num_vertices + 2, num_vertices + 3
							};
							mesh->vertices.insert(mesh->vertices.begin(), std::begin(right), std::end(right));
							mesh->indices.insert(mesh->indices.begin(), std::begin(new_indices), std::end(new_indices));
						}
						// top
						if ((y < (pc::c_height - 1) && chunk->blocks[x][y + 1][z].id == 0)) {
							float top[] = {
								pos.x - 0.5f, pos.y + 0.5f, pos.z - 0.5f, block_uvs[4][0].x, block_uvs[4][1].y, 1.0f, // bottom left
								pos.x + 0.5f, pos.y + 0.5f, pos.z - 0.5f, block_uvs[4][1].x, block_uvs[4][1].y, 1.0f, // bottom right
								pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f, block_uvs[4][1].x, block_uvs[4][0].y, 1.0f, // top right
								pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.5f, block_uvs[4][0].x, block_uvs[4][0].y, 1.0f // top left
							};
							int num_vertices = mesh->vertices.size() / 6;
							int new_indices[] = {
								num_vertices, num_vertices + 1, num_vertices + 2,
								num_vertices, num_vertices + 2, num_vertices + 3
							};
							mesh->vertices.insert(mesh->vertices.begin(), std::begin(top), std::end(top));
							mesh->indices.insert(mesh->indices.begin(), std::begin(new_indices), std::end(new_indices));
						}
						// bottom
						if (!prev_full && (y > 0 && chunk->blocks[x][y - 1][z].id == 0)) {
							float bottom[] = {
								pos.x - 0.5f, pos.y - 0.5f, pos.z + 0.5f, block_uvs[5][0].x, block_uvs[5][1].y, bottom_light, // bottom left
								pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.5f, block_uvs[5][1].x, block_uvs[5][1].y, bottom_light, // bottom right
								pos.x + 0.5f, pos.y - 0.5f, pos.z - 0.5f, block_uvs[5][1].x, block_uvs[5][0].y, bottom_light, // top right
								pos.x - 0.5f, pos.y - 0.5f, pos.z - 0.5f, block_uvs[5][0].x, block_uvs[5][0].y, bottom_light  // top left
							};
							int num_vertices = mesh->vertices.size() / 6;
							int new_indices[] = {
								num_vertices, num_vertices + 1, num_vertices + 2,
								num_vertices, num_vertices + 2, num_vertices + 3
							};
							mesh->vertices.insert(mesh->vertices.begin(), std::begin(bottom), std::end(bottom));
							mesh->indices.insert(mesh->indices.begin(), std::begin(new_indices), std::end(new_indices));
						}
					}
					else {
						full = false;
					}
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			if (adj_chunks[i] != nullptr) {
				adj_chunks[i]->lock.unlock();
			}
		}
		chunk->state = Chunk::State::MESH;
	}
}