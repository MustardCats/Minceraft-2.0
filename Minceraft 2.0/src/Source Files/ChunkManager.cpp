#include "../Header Files/ChunkManager.h"

namespace ChunkManager {
	// TODO: change to unordered map
	static std::vector<Chunk*> chunks; // normal chunks
	static std::vector<Chunk*> generating_chunks; // chunks being created
	static std::vector<Chunk*> mesh_chunks; // chunks to be sent to the renderer to make meshes
	static std::vector<glm::ivec3> delete_chunks; // positions of chunk meshes to be deleted
	static FastNoiseLite height_noise;
	static short range_x;
	static short range_y;
	static short range_z;

	bool init() {
		range_x = 13;
		range_y = 3;
		range_z = 13;

		//height_noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);

		return true;
	}

	std::array<std::array<int, c_width>, c_length> getHeightMap(int chunk_x, int chunk_z) {
		std::array<std::array<int, c_width>, c_length> height_map = std::array<std::array<int, c_width>, c_length>();
		for (int x = 0; x < c_length; x++) {
			for (int z = 0; z < c_width; z++) {
				height_map[x][z] = (int)(height_noise.GetNoise((float)chunk_x * c_length + x, (float)chunk_z * c_width + z) * 10.0f);
			}
		}
		
		return height_map;
	}

	void generateChunk(Chunk* chunk) {
		std::array<std::array<int, c_width>, c_length> height_map = getHeightMap(chunk->x, chunk->z);
		for (int x = 0; x < c_length; x++) {
			for (int y = 0; y < c_height; y++) {
				for (int z = 0; z < c_width; z++) {
					if ((chunk->y * c_height + y) < height_map[x][z]) {
						chunk->at(x, y, z)->id = 2;
					}
					else if ((chunk->y * c_height + y) == height_map[x][z]) {
						chunk->at(x, y, z)->id = 4;
					}
					/*
					if (z != 0 && x != 0 && y != 0 && z != 15 && x != 15 && y != 15) {
						if (x % 2 == 0)
							chunk->at(x, y, z)->id = 4;
						else
							chunk->at(x, y, z)->id = 2;
					}
					*/
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
				delete_chunks.push_back(glm::ivec3(chunks.at(i)->x, chunks.at(i)->y, chunks.at(i)->z));
				delete chunks.at(i);
				chunks.erase(chunks.begin() + i);
				return;
			}
		}
		for (int i = 0; i < mesh_chunks.size(); i++) {
			if (mesh_chunks.at(i)->x < (chunk_pos.x - (range_x / 2)) ||
				mesh_chunks.at(i)->x >(chunk_pos.x + (range_x / 2)) ||
				mesh_chunks.at(i)->y < (chunk_pos.y - (range_y / 2)) ||
				mesh_chunks.at(i)->y >(chunk_pos.y + (range_y / 2)) ||
				mesh_chunks.at(i)->z < (chunk_pos.z - (range_z / 2)) ||
				mesh_chunks.at(i)->z >(chunk_pos.z + (range_z / 2))) {
				mesh_chunks.erase(mesh_chunks.begin() + i);
			}
		}
	}
	// TODO: change to glm::ivec3
	Chunk* getChunk(int x, int y, int z) {
		for (auto chunk : chunks) {
			if (chunk->x == x && chunk->y == y && chunk->z == z)
				return chunk;
		}

		return nullptr;
	}

	Chunk* getNewChunk() {
		if (mesh_chunks.size() < 1)
			return nullptr;
		Chunk* chunk = mesh_chunks.at(0);
		mesh_chunks.erase(mesh_chunks.begin());

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
		glm::ivec3 pos = delete_chunks.at(0);
		delete_chunks.erase(delete_chunks.begin());

		return pos;
	}

	bool containsDeleteChunks() {
		return delete_chunks.size() > 0;
	}
}