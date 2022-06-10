#include "ChunkWorker.h"

namespace ChunkWorker {

	static bool is_active = false;
	static bool modify_chunks = false;
	static bool chunk_finished = false;
	static bool pause_mode = false;
	static bool paused = true;
	static Chunk* generating_chunk = nullptr;
	static glm::ivec3 prev_chunk_pos = glm::ivec3(0);
	static glm::vec3* player_pos = nullptr; // when player leaves chunk, empty_chunks must be updated
	static glm::vec3 old_player_pos = glm::vec3(1000.0f);
	static std::queue<glm::ivec3> empty_chunks;
	static std::queue<glm::ivec3> regen_chunks;
	static std::mutex regen_lock;
	// may be deprecated
	static short range_x = 21;
	static short range_y = 5;
	static short range_z = 21;
	// probably will be used
	static float radius;

	void deleteChunks() {
		glm::ivec3 chunk_pos = ChunkManager::posToChunk(*player_pos);
		if (ChunkManager::chunkLock()->try_lock()) {
			if (pause_mode)
				std::cout << "DELETE: Looking for chunks out of range\n";
			robin_hood::unordered_map<glm::ivec3, Chunk*>* chunks = ChunkManager::getChunks();
			robin_hood::unordered_map<glm::ivec3, Chunk*>::iterator it;
			for (it = chunks->begin(); it != chunks->end(); it++) {
				if ((*it).first.x < (chunk_pos.x - (range_x / 2)) ||
					(*it).first.x > (chunk_pos.x + (range_x / 2)) ||
					(*it).first.z < (chunk_pos.z - (range_z / 2)) ||
					(*it).first.z > (chunk_pos.z + (range_z / 2)))
				{
					glm::vec3 chunk_block_pos = glm::vec3((*it).first.x, 0, (*it).first.z) *
						glm::vec3(pc::c_length, 0, pc::c_width) + (glm::vec3(pc::c_length, 0, pc::c_width) / 2.0f);
					Chunk* chunk = it->second;
					if (chunk != nullptr && chunk->state != Chunk::State::DELETING) {
						if (chunk->lock.try_lock()) {
							if (pause_mode)
								std::cout << "DELETE: Saving and setting chunk to delete for chunk " << chunk->toString() << "\n";
							if (chunk->is_modified)
								ChunkManager::serialize(chunk);
							chunk->state = Chunk::State::DELETING;
							chunk->lock.unlock();
						}
					}
				}
			}
			ChunkManager::chunkLock()->unlock();
		}
	}

	bool regenNearMeshes(glm::ivec3 pos) {
		Chunk* adj_chunks[4]{};
		if (!ChunkManager::chunkLock()->try_lock()) {
			return false;
		}
		adj_chunks[0] = ChunkManager::getChunk(pos + glm::ivec3(-1, 0, 0));
		adj_chunks[1] = ChunkManager::getChunk(pos + glm::ivec3(1, 0, 0));
		adj_chunks[2] = ChunkManager::getChunk(pos + glm::ivec3(0, 0, -1));
		adj_chunks[3] = ChunkManager::getChunk(pos + glm::ivec3(0, 0, 1));
		for (int i = 0; i < 4; i++) {
			if (adj_chunks[i] != nullptr && adj_chunks[i]->state != Chunk::State::DELETING) {
				while (!adj_chunks[i]->lock.try_lock()) {
					
				}
				if (pause_mode)
					std::cout << "MESH BUILDING: Building mesh for chunk " << adj_chunks[i]->toString() << "\n";
				ChunkMeshBuilder::buildChunkMesh(adj_chunks[i]);
				adj_chunks[i]->lock.unlock();
			}
		}
		ChunkManager::chunkLock()->unlock();

		return true;
	}

	void generateChunks() {
		if (generating_chunk == nullptr) {
			if (pause_mode)
				std::cout << "GENERATING: Attempting to generate new chunk\n";
			if (ChunkManager::posToChunk(*player_pos) != ChunkManager::posToChunk(old_player_pos))
			{
				// problem here
				if (pause_mode)
					std::cout << "GENERATING: Player left chunk, finding new empty chunks\n";
				if (ChunkManager::chunkLock()->try_lock()) {
					old_player_pos = *player_pos;
					empty_chunks = ChunkManager::findEmptyChunks(ChunkManager::posToChunk(*player_pos));
					ChunkManager::chunkLock()->unlock();
				}
			}
			if (!empty_chunks.empty()) {				
				chunk_finished = false;
				generating_chunk = new Chunk(empty_chunks.front());
				empty_chunks.pop();
				if (pause_mode)
					std::cout << "GENERATING: Found empty space and creating a new chunk at " << generating_chunk->toString() << "\n";
				if (!ChunkManager::deserialize(generating_chunk)) {
					ChunkManager::generateChunk(generating_chunk);
				}
				chunk_finished = true;
				return;
			}
		}
	}

	void update() {
		is_active = true;

		while (modify_chunks) {
			if (player_pos != nullptr && !ChunkManager::deletingChunks() && !ChunkRenderer::rendering()) {
				// for examining the worker frame by frame
				if (pause_mode) {
					paused = true;
					while (paused) {
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
					std::cout << "-----Unpausing Chunk Worker-----\n";
				}
				generateChunks();
				if (!ChunkRenderer::rendering()) {
					if (generating_chunk != nullptr) {
						if (ChunkManager::addChunk(generating_chunk)) {
							if (pause_mode)
								std::cout << "GENERATING: Adding new chunk to chunk manager " << generating_chunk->toString() << "\n";
							regen_chunks.push(generating_chunk->pos);
							generating_chunk = nullptr;
						}
					}
				}
				//std::this_thread::sleep_for(std::chrono::milliseconds(3));
				if (regen_chunks.size() > 0 && !ChunkRenderer::rendering()) {
					if (regenNearMeshes(regen_chunks.front()))
						regen_chunks.pop();
				}
				//std::this_thread::sleep_for(std::chrono::milliseconds(3));
				deleteChunks();
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		is_active = false;
	}

	void start() {
		if (modify_chunks)
			return;
		// need to wait until thread exits loop
		// so multiple threads aren't created
		while (is_active) {
			
		}
		modify_chunks = true;
		std::thread worker(update);
		worker.detach();
	}

	void end() {
		modify_chunks = false;
	}

	bool isActive() {
		return is_active;
	}

	Chunk* getNewChunk() {
		if (!chunk_finished) {
			return nullptr;
		}
		Chunk* temp = generating_chunk;
		generating_chunk = nullptr;
		return temp;
	}

	void setPlayerPos(glm::vec3* player_pos) {
		ChunkWorker::player_pos = player_pos;
	}

	void togglePauseMode() {
		pause_mode = 1 - pause_mode;
	}

	void unpause() {
		paused = false;
	}
}