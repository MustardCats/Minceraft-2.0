#include "ChunkManager.h"

namespace ChunkManager {

	static robin_hood::unordered_map<glm::ivec3, Chunk*> chunks; // normal chunks
	static std::mutex chunk_lock; // for deleting chunks on main thread
	static std::queue<glm::ivec3> delete_chunks; // chunks to be deleted
	static FastNoiseLite height_noise;
	// may be deprecated
	static short range_x;
	static short range_y;
	static short range_z;
	// probably will be used
	static float radius;
	static bool first_frame;
	static bool deleting_chunks = false;
	static std::string chunk_directory;

	bool init() {
		range_x = 21;
		range_y = 5;
		range_z = 21;
		radius = 0.0f;
		first_frame = true;
		//height_noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
		chunk_directory = "saves/World/chunks/";
		ChunkMeshBuilder::setChunks(&chunks);
		deleting_chunks = false;

		return true;
	}

	void deleteEmptyChunks() {
		deleting_chunks = true;
		if (chunk_lock.try_lock()) {
			for (auto& pair : chunks) {
				if (pair.second == nullptr) {
					chunks.erase(pair.first);
				}
				else if (pair.second->state == Chunk::State::DELETING) {
					if (pair.second->lock.try_lock()) {
						pair.second->lock.unlock();
						delete pair.second;
						chunks.erase(pair.first);
					}
				}
			}
			chunk_lock.unlock();
		}
		deleting_chunks = false;
	}

	void generateChunkBuffers() {
		if (chunk_lock.try_lock()) {
			for (auto& pair : chunks) {
				if (pair.second) {
					if (pair.second->lock.try_lock()) {
						if (pair.second->state == Chunk::State::MESH) {
							pair.second->mesh.generateBuffers();
							pair.second->state = Chunk::State::BUFFERS;
						}
						pair.second->lock.unlock();
					}
				}
			}
			chunk_lock.unlock();
		}
	}

	void update() {
		generateChunkBuffers();
		deleteEmptyChunks();
	}
	// https://stackoverflow.com/review/suggested-edits/1416384
	std::queue<glm::ivec3> findEmptyChunks(glm::ivec3 chunk_pos) {
		std::queue<glm::ivec3> empty_chunks;
		int X = range_x;
		int Y = range_z;
		int x, y, dx, dy;
		x = y = dx = 0;
		dy = -1;
		int t = std::max(X, Y);
		int maxI = t * t;
		for (int i = 0; i < maxI; i++) {
			if ((-X / 2 <= x) && (x <= X / 2) && (-Y / 2 <= y) && (y <= Y / 2)) {
				glm::ivec3 pos = glm::ivec3(x, 0, y) + chunk_pos;
				if (getChunk(pos) == nullptr) {
					empty_chunks.push(pos);
				}
			}
			if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))) {
				t = dx;
				dx = -dy;
				dy = t;
			}
			x += dx;
			y += dy;
		}

		return empty_chunks;
	}

	std::array<std::array<int, pc::c_width>, pc::c_length> getHeightMap(int chunk_x, int chunk_z) {
		std::array<std::array<int, pc::c_width>, pc::c_length> height_map = std::array<std::array<int, pc::c_width>, pc::c_length>();
		for (int x = 0; x < pc::c_length; x++) {
			for (int z = 0; z < pc::c_width; z++) {
				height_map[x][z] = (int)(height_noise.GetNoise((float)chunk_x * pc::c_length + x, (float)chunk_z * pc::c_width + z) * 10.0f + 20.0f);
			}
		}

		return height_map;
	}

	void serialize(Chunk* chunk) {
		std::stringstream file_name;
		file_name << chunk_directory << chunk->pos.x << " " << chunk->pos.y << " " << chunk->pos.z << ".txt";
		FILE* file = fopen(file_name.str().c_str(), "wb");
		if (file) {
			fwrite(chunk->blocks, sizeof(short), pc::c_length * pc::c_height * pc::c_width, file);
			fclose(file);
		}
		else {
			std::cout << "ERROR: couldn't save chunk to " << file_name.str() << "\n";
		}
	}

	bool deserialize(Chunk* chunk) {
		std::stringstream file_name;
		file_name << chunk_directory << chunk->pos.x << " " << chunk->pos.y << " " << chunk->pos.z << ".txt";
		FILE* file = fopen(file_name.str().c_str(), "rb");
		if (file) {
			fread(chunk->blocks, sizeof(short), pc::c_length * pc::c_height * pc::c_width, file);
			fclose(file);
			chunk->state = Chunk::State::GENERATED;
			return true;
		}
		return false;
	}

	void generateChunk(Chunk* chunk) {
		std::array<std::array<int, pc::c_width>, pc::c_length> height_map = getHeightMap(chunk->pos.x, chunk->pos.z);
		for (int x = 0; x < pc::c_length; x++) {
			for (int y = 0; y < pc::c_height; y++) {
				for (int z = 0; z < pc::c_width; z++) {
					if ((chunk->pos.y * pc::c_height + y) == height_map[x][z]) {
						chunk->blocks[x][y][z].id = 3;
					}
					else if ((chunk->pos.y * pc::c_height + y) > height_map[x][z] - 4 &&
							 (chunk->pos.y * pc::c_height + y) < height_map[x][z]) {
						chunk->blocks[x][y][z].id = 1;
					}
					else if ((chunk->pos.y * pc::c_height + y) <= height_map[x][z] - 4) {
						chunk->blocks[x][y][z].id = 2;
					}
					/*
					if (z != 0 && x != 0 && y != 0 && z != 15 && x != 15 && y != 15) {
						if (x % 2 == 0)
							chunk->blocks[x][y][z].id = 4;
						else
							chunk->blocks[x][y][z].id = 2;
					}
					*/
				}
			}
		}
		chunk->state = Chunk::State::GENERATED;
	}

	void deleteChunk(glm::ivec3 pos) {
		if (chunks.find(pos) != chunks.end()) {
			delete_chunks.push(pos);
		}
	}
	
	Chunk* getChunk(glm::ivec3 pos) {
		if (chunks.find(pos) != chunks.end()) {
			return chunks[pos];
		}
		return nullptr;
	}

	Block* getBlock(glm::ivec3 pos) {
		Chunk* chunk = getChunk(posToChunk(pos));
		if (chunk == nullptr) {
			return nullptr;
		}
		glm::ivec3 indices = chunkRelative(pos);
		return &chunk->blocks[indices.x][indices.y][indices.z];
	}

	bool chunkExists(glm::ivec3 pos) {
		return chunks.find(pos) != chunks.end();
	}

	std::mutex* chunkLock() {
		return &chunk_lock;
	}

	bool addChunk(Chunk* chunk) {
		if (!chunk) {
			return false;
		}
		if (chunk_lock.try_lock()) {
			glm::ivec3 pos = chunk->pos;
			if (!chunks.contains(pos)) {
				chunks.emplace(pos, chunk);
				chunk_lock.unlock();
				return true;
			}
			chunk_lock.unlock();
		}
		return false;
	}

	robin_hood::unordered_map<glm::ivec3, Chunk*>* getChunks() {
		return &chunks;
	}

	std::array<Chunk*, 4> findAdjacentChunks(Chunk* chunk) {
		std::array<Chunk*, 4> adj_chunks{ nullptr };
		if (chunk != nullptr) {
			adj_chunks[0] = getChunk(chunk->pos + glm::ivec3(-1, 0, 0));
			adj_chunks[1] = getChunk(chunk->pos + glm::ivec3(1, 0, 0));
			adj_chunks[2] = getChunk(chunk->pos + glm::ivec3(0, 0, -1));
			adj_chunks[3] = getChunk(chunk->pos + glm::ivec3(0, 0, 1));
		}
		return adj_chunks;
	}

	void changeBlock(glm::ivec3 pos, int id) {
		Chunk* chunk = getChunk(posToChunk(pos));
		if (chunk != nullptr) {
			glm::ivec3 indices = chunkRelative(pos);
			chunk->blocks[indices.x][indices.y][indices.z].id = id;
			ChunkMeshBuilder::buildChunkMesh(chunk);
			chunk->mesh.generateBuffers();
			// regen adjacent chunk
			glm::ivec3 chunk_relative = chunkRelative(pos);
			std::array<Chunk*, 4> adj_chunks = findAdjacentChunks(chunk);
			if (chunk_relative.x == 0 && adj_chunks[0] != nullptr)
			{
				if (adj_chunks[0]->blocks[pc::c_length - 1][chunk_relative.y][chunk_relative.z].id != 0) {
					ChunkMeshBuilder::buildChunkMesh(adj_chunks[0]);
					adj_chunks[0]->mesh.generateBuffers();
				}
			}
			if (chunk_relative.x == pc::c_length - 1 && adj_chunks[1] != nullptr)
			{
				if (adj_chunks[1]->blocks[0][chunk_relative.y][chunk_relative.z].id != 0) {
					ChunkMeshBuilder::buildChunkMesh(adj_chunks[1]);
					adj_chunks[1]->mesh.generateBuffers();
				}
			}
			if (chunk_relative.z == 0 && adj_chunks[2] != nullptr)
			{
				if (adj_chunks[2]->blocks[chunk_relative.x][chunk_relative.y][pc::c_width - 1].id != 0) {
					ChunkMeshBuilder::buildChunkMesh(adj_chunks[2]);
					adj_chunks[2]->mesh.generateBuffers();
				}
			}
			if (chunk_relative.z == pc::c_width - 1 && adj_chunks[3] != nullptr)
			{
				if (adj_chunks[3]->blocks[chunk_relative.x][chunk_relative.y][0].id != 0) {
					ChunkMeshBuilder::buildChunkMesh(adj_chunks[3]);
					adj_chunks[3]->mesh.generateBuffers();
				}
			}
			chunk->is_modified = true;
		}
	}

	glm::ivec3 posToChunk(glm::ivec3 pos) {
		glm::ivec3 chunk_pos;
		chunk_pos.x = floor((float)pos.x / pc::c_length);
		chunk_pos.y = floor((float)pos.y / pc::c_height);
		chunk_pos.z = floor((float)pos.z / pc::c_width);

		return chunk_pos;
	}
	
	glm::ivec3 chunkRelative(glm::ivec3 pos) {
		glm::ivec3 relative;
		relative.x = pos.x % pc::c_length;
		relative.y = pos.y % pc::c_height;
		relative.z = pos.z % pc::c_width;
		if (relative.x < 0) {
			relative.x += pc::c_length;
		}
		if (relative.y < 0) {
			relative.y += pc::c_height;
		}
		if (relative.z < 0) {
			relative.z += pc::c_width;
		}
		return relative;
	}

	void flushChunks() {
		while (!chunk_lock.try_lock()) {

		}
		for (auto& pair : chunks) {
			if (pair.second != nullptr) {
				while (!pair.second->lock.try_lock()) {

				}
				if (pair.second->is_modified) {
					serialize(pair.second);
					std::cout << pair.first.x << " " << pair.first.y << " " << pair.first.z << "\n";
				}
				pair.second->lock.unlock();
				delete pair.second;
			}
		}
		chunks.clear();
		chunk_lock.unlock();
	}

	bool deletingChunks() {
		return deleting_chunks;
	}
}