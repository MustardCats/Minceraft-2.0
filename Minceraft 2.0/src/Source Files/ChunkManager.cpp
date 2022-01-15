#include "../Header Files/ChunkManager.h"

namespace ChunkManager {

	static std::vector<Chunk*> chunks; // normal chunks
	static std::vector<Chunk*> generating_chunks; // chunks being created
	static std::vector<Chunk*> mesh_chunks; // chunks to be sent to the renderer to make meshes
	static std::vector<glm::ivec3> delete_chunks; // positions of chunk meshes to be deleted
	static short range_x;
	static short range_y;
	static short range_z;

	bool init() {
		range_x = 3;
		range_y = 3;
		range_z = 3;

		return true;
	}

	void generateChunk(Chunk* chunk) {
		for (int x = 0; x < c_length; x++) {
			for (int y = 0; y < c_height; y++) {
				for (int z = 0; z < c_width; z++) {
					if (z != 0 && x != 0 && y != 0 && z != 15 && x != 15 && y != 15) {
						if (x % 2 == 0)
							chunk->at(x, y, z)->id = 4;
						else
							chunk->at(x, y, z)->id = 2;
					}
				}
			}
		}
	}

	void update() {
		if (generating_chunks.size() > 0) {
			generateChunk(generating_chunks.at(generating_chunks.size() - 1));
			chunks.push_back(generating_chunks.at(generating_chunks.size() - 1));
			generating_chunks.erase(generating_chunks.end() - 1);
			// tell the client to make a mesh
			mesh_chunks.push_back(chunks.at(chunks.size() - 1));
		}
	}

	void makeChunkNear(glm::vec3 pos) {
		bool found = false;
		glm::vec3 chunk_pos = posToChunk(pos);
		for (int x = (chunk_pos.x - (range_x / 2)); x <= (chunk_pos.x + (range_x / 2)); x++) {
			for (int y = (chunk_pos.y - (range_y / 2)); y <= (chunk_pos.y + (range_y / 2)); y++) {
				for (int z = (chunk_pos.z - (range_z / 2)); z <= (chunk_pos.z + (range_z / 2)); z++) {
					found = false;
					for (auto chunk : generating_chunks) {
						if (chunk->x == x && chunk->y == y && chunk->z == z)
							found = true;
					}
					if (found)
						continue;
					for (auto chunk : chunks) {
						if (chunk->x == x && chunk->y == y && chunk->z == z)
							found = true;
					}
					if (found)
						continue;
					std::cout << "Making new chunk at " << x << " " << y << " " << z << "\n";
					generating_chunks.push_back(new Chunk(x, y, z));
				}
			}
		}
	}

	void deleteChunks(glm::vec3 pos) {
		glm::vec3 chunk_pos = posToChunk(pos);
		for (int i = 0; i < chunks.size(); i++) {
			if (chunks.at(i)->x < (chunk_pos.x - (range_x / 2)) ||
				chunks.at(i)->x > (chunk_pos.x + (range_x / 2)) ||
				chunks.at(i)->y < (chunk_pos.y - (range_y / 2)) ||
				chunks.at(i)->y >(chunk_pos.y + (range_y / 2)) ||
				chunks.at(i)->z < (chunk_pos.z - (range_z / 2)) ||
				chunks.at(i)->z >(chunk_pos.z + (range_z / 2))) {
				delete chunks.at(i);
				chunks.erase(chunks.begin() + i);
				delete_chunks.push_back(glm::ivec3(chunks.at(i)->x, chunks.at(i)->y, chunks.at(i)->z));
				return;
			}
		}
	}

	Chunk* getNewChunk() {
		if (mesh_chunks.size() < 1)
			return nullptr;
		Chunk* chunk = mesh_chunks.at(mesh_chunks.size() - 1);
		mesh_chunks.pop_back();

		return chunk;
	}

	glm::ivec3 posToChunk(glm::ivec3 pos) {
		glm::ivec3 chunk_pos;
		chunk_pos.x = floor((float)pos.x / c_length);
		chunk_pos.y = floor((float)pos.y / c_height);
		chunk_pos.z = floor((float)pos.z / c_width);

		return chunk_pos;
	}

	glm::ivec3 getDeleteChunk() {
		glm::ivec3 pos = delete_chunks.at(delete_chunks.size() - 1);
		delete_chunks.pop_back();

		return pos;
	}

	bool containsDeleteChunks() {
		return delete_chunks.size() > 0;
	}
}