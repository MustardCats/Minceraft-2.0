#include "Game.h"

namespace Game {

	static std::vector<Player*> players;

	void exit() {
		ChunkWorker::end();
		ChunkManager::flushChunks();
	}

	bool init() {
		if (!loadResources())
			return false;

		double delta_time = 0.0f;
		while (true) {
			auto start_time = std::chrono::high_resolution_clock::now();
			if (!gameLoop(delta_time))
				break;
			auto end_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
			delta_time = (double)((duration.count()) / 1000000.0);
			// calculate FPS
			static int count = 0;
			static float total_time = 0;
			total_time += delta_time;
			// display FPS once per second
			count++;
			if (total_time > 1.0) {
				total_time = 0;
				std::string title = "FPS: " + std::to_string(count);
				Window::setWindowTitle(title);
				count = 0;
			}
		}

		exit();

		return true;
	}

	bool loadResources() {
		if (!Window::init()) {
			printf("ERROR: The window couldn't be initialized!\n");
		}
		if (!BlockTypes::init("Assets/")) {
			printf("ERROR: The block types couldn't be initialized!\n");
		}
		if (!Input::init()) {
			printf("ERROR: The input couldn't be initialized!\n");
		}
		if (!Renderer::init()) {
			printf("ERROR: The renderer couldn't be initialized!\n");
		}
		if (!Camera::init()) {
			printf("ERROR: The camera couldn't be initialized!\n");
		}
		if (!ChunkManager::init()) {
			printf("ERROR: The chunk manager couldn't be initialized!\n");
		}
		players.push_back(new Player());
		InputHandler::setPlayer(players[0]);
		UIRenderer::setPlayer(players[0]);
		DebugRenderer::setPlayer(players[0]);
		ChunkRenderer::setChunks(ChunkManager::getChunks());
		ChunkWorker::setPlayerPos(&players[0]->getPos());
		ChunkWorker::start();
		//ChunkWorker::togglePauseMode();

		return true;
	}

	bool gameLoop(double delta_time) {
		// resets previous inputs
		// must be done before event polling
		Input::update();
		if (!Window::update()) {
			return false;
		}
		InputHandler::update(delta_time);
		// TODO: change to entities list, where player is inside of
		for (auto player : players) {
			player->update(delta_time);
		}
		/*
		static Chunk* chunk = nullptr;
		if (chunk == nullptr) {
			chunk = ChunkWorker::getNewChunk();
		}
		if (chunk != nullptr && false) {
			if (ChunkManager::addChunk(chunk)) {
				chunk = nullptr;
			}
		}
		*/
		ChunkManager::update();
		
		Renderer::render();

		return true;
	}
}
