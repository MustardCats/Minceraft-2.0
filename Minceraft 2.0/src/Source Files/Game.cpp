#include "..\Header Files\Game.h"

namespace Game {

	static std::vector<Player*> players;

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
			delta_time = (float)((duration.count()) / 1000000.0);
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
		return true;
	}

	bool loadResources() {
		if (!Window::init()) {
			printf("ERROR: The window couldn't be initialized!");
		}
		if (!Input::init()) {
			printf("ERROR: The input couldn't be initialized!");
		}
		if (!Renderer::init()) {
			printf("ERROR: The renderer couldn't be initialized!");
		}
		if (!Camera::init()) {
			printf("ERROR: The camera couldn't be initialized!");
		}
		if (!UIManager::init()) {
			printf("ERROR: The UI manager couldn't be initialized!");
		}
		players.push_back(new Player());
		InputHandler::setPlayer(players[0]);

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

		Renderer::render();

		return true;
	}
}