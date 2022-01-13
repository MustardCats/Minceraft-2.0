#include "../Header Files/InputHandler.h"

namespace InputHandler {

	static Player* player = nullptr;

	void movePlayerForward(double delta_time, float offset) {
		player->addPos(Camera::getForward() * (float)delta_time * offset);
	}

	void movePlayerBack(double delta_time, float offset) {
		player->addPos(-Camera::getForward() * (float)delta_time * offset);
	}

	void movePlayerLeft(double delta_time, float offset) {
		player->addPos(-Camera::getRight() * (float)delta_time * offset);
	}

	void movePlayerRight(double delta_time, float offset) {
		player->addPos(Camera::getRight() * (float)delta_time * offset);
	}

	void movePlayerUp(double delta_time, float offset) {
		player->addPos(glm::vec3(0.0f, 1.0f, 0.0f) * (float)delta_time * offset);
	}

	void movePlayerDown(double delta_time, float offset) {
		player->addPos(-glm::vec3(0.0f, 1.0f, 0.0f) * (float)delta_time * offset);
	}

	void updatePlayerMovement(double delta_time) {
		if (Input::keyHold(GLFW_KEY_W)) {
			movePlayerForward(delta_time, 1.0f);
		}
		if (Input::keyHold(GLFW_KEY_S)) {
			movePlayerBack(delta_time, 1.0f);
		}
		if (Input::keyHold(GLFW_KEY_A)) {
			movePlayerLeft(delta_time, 1.0f);
		}
		if (Input::keyHold(GLFW_KEY_D)) {
			movePlayerRight(delta_time, 1.0f);
		}
		if (Input::keyHold(GLFW_KEY_SPACE)) {
			movePlayerUp(delta_time, 1.0f);
		}
		if (Input::keyHold(GLFW_KEY_LEFT_SHIFT)) {
			movePlayerDown(delta_time, 1.0f);
		}
		if (Input::keyPress(GLFW_KEY_F)) {
			std::cout << player->getPos().x << " " << player->getPos().y << " " << player->getPos().z << "\n";
		}
	}

	void updateCamera() {
		static glm::ivec2 old_mouse = Input::getMouse();

		float xoffset = old_mouse.x - Input::getMouse().x;
		float yoffset = old_mouse.y - Input::getMouse().y;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		Camera::addYaw(xoffset);
		Camera::addPitch(yoffset);

		old_mouse = Input::getMouse();
	}

	void update(double delta_time) {
		if (player != nullptr) {
			updateCamera();
			updatePlayerMovement(delta_time);
			Camera::setPosition(player->getPos());
		}
	}
	
	void setPlayer(Player* player) {
		InputHandler::player = player;
	}
}