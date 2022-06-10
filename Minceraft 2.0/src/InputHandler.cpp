#include "InputHandler.h"

namespace InputHandler {

	static Player* player = nullptr;
	static bool first_person = true;

	void movePlayerForward(double delta_time, float offset) {
		//player->addPos(Camera::getForward() * (float)delta_time * offset);
		player->move(Camera::getForward());
	}

	void movePlayerBack(double delta_time, float offset) {
		//player->addPos(-Camera::getForward() * (float)delta_time * offset);
		player->move(-Camera::getForward());
	}

	void movePlayerLeft(double delta_time, float offset) {
		//player->addPos(-Camera::getRight() * (float)delta_time * offset);
		player->move(-Camera::getRight());
	}

	void movePlayerRight(double delta_time, float offset) {
		//player->addPos(Camera::getRight() * (float)delta_time * offset);
		player->move(Camera::getRight());
	}

	void movePlayerUp(double delta_time, float offset) {
		player->move(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void movePlayerDown(double delta_time, float offset) {
		player->move(glm::vec3(0.0f, -1.0f, 0.0f));
	}

	void updatePlayerMovement(double delta_time) {
		float velocity = 7.0f;
		const float MAX_FOV = 110.0f;
		const float DEFAULT_FOV = 90.0f;
		static float fov = DEFAULT_FOV;
		if (Input::keyHold(GLFW_KEY_LEFT_CONTROL)) {
			player->run();
			fov += 220.0f * delta_time;
			if (fov > MAX_FOV) {
				fov = MAX_FOV;
			}
		}
		else {
			fov -= 220.0f * delta_time;
			if (fov < DEFAULT_FOV) {
				fov = DEFAULT_FOV;
			}
		}
		if (Input::keyHold(GLFW_KEY_C)) {
			fov = 30.0f;
		}
		Camera::setFOV(fov);
		if (Input::keyHold(GLFW_KEY_W)) {
			movePlayerForward(delta_time, velocity);
		}
		if (Input::keyHold(GLFW_KEY_S)) {
			movePlayerBack(delta_time, velocity);
		}
		if (Input::keyHold(GLFW_KEY_A)) {
			movePlayerLeft(delta_time, velocity);
		}
		if (Input::keyHold(GLFW_KEY_D)) {
			movePlayerRight(delta_time, velocity);
		}
		if (Input::keyHold(GLFW_KEY_SPACE)) {
			movePlayerUp(delta_time, velocity);
		}
		if (Input::keyHold(GLFW_KEY_LEFT_SHIFT)) {
			movePlayerDown(delta_time, velocity);
		}
		if (Input::keyPress(GLFW_KEY_E)) {
			player->toggleInv();
			if (player->isInvOpen()) {
				Window::unlockMouse();
			}
			else {
				Window::lockMouse();
			}
		}
		if (Input::keyPress(GLFW_KEY_ESCAPE)) {
			if (Window::isMouseLocked()) {
				Window::unlockMouse();
			}
			else {
				Window::lockMouse();
			}
		}
		if (Input::keyPress(GLFW_KEY_F)) {
			if (first_person) {
				first_person = false;
			}
			else {
				first_person = true;
			}
		}
		if (Input::keyPress(GLFW_KEY_P)) {
			glm::ivec3 chunk_pos = ChunkManager::posToChunk(round(player->getPos()));
			glm::ivec3 relative = ChunkManager::chunkRelative(round(player->getPos()));
			Chunk* chunk = ChunkManager::getChunk(chunk_pos);
			std::cout << "Position: " << player->getPos().x << " " << player->getPos().y << " " << player->getPos().z <<
				" Chunk: " << chunk_pos.x << " " << chunk_pos.y << " " << chunk_pos.z <<
				" Relative: " << relative.x << " " << relative.y << " " << relative.z;
			if (chunk != nullptr) {
				std::cout << " Chunk State: " << (int)chunk->state << " " << chunk->mesh.vertices.size() << "\n";
			}

		}
		if (Input::keyPress(GLFW_KEY_R)) {
			DebugRenderer::toggleRigidBodies();
		}
		if (Input::keyPress(GLFW_KEY_M)) {
			glm::ivec3 chunk_pos = ChunkManager::posToChunk(round(player->getPos()));
			Chunk* chunk = ChunkManager::getChunk(chunk_pos);
			if (chunk != nullptr) {
				ChunkMeshBuilder::buildChunkMesh(chunk);
			}
		}
		if (Input::keyPress(GLFW_KEY_U)) {
			ChunkWorker::unpause();
		}
		if (Input::keyHold(GLFW_KEY_I)) {
			ChunkWorker::unpause();
		}
		if (Input::keyPress(GLFW_KEY_B)) {
			glm::ivec3 chunk_pos = ChunkManager::posToChunk(round(player->getPos()));
			Chunk* chunk = ChunkManager::getChunk(chunk_pos);
			if (chunk != nullptr) {
				chunk->mesh.generateBuffers();
			}
		}
		if (Input::mousePress(GLFW_MOUSE_BUTTON_LEFT)) {
			player->tryBreakBlock();
			player->leftClickInv(Input::getMouse());
		}
		if (Input::mousePress(GLFW_MOUSE_BUTTON_RIGHT)) {
			player->tryPlaceBlock();
			player->rightClickInv(Input::getMouse());
		}
	}

	void updateCamera(float delta_time) {
		if (Window::isMouseLocked()) {
			glm::vec2 offset = Input::getPrevMouse() - Input::getMouse();
			const float sensitivity = 0.1f;
			offset *= sensitivity;
			Camera::addYaw(offset.x);
			Camera::addPitch(offset.y);
		}
		if (!first_person) {
			float camera_speed = 5.0f;
			if (Input::keyHold(GLFW_KEY_J)) {
				Camera::moveLeft(camera_speed * delta_time);
			}
			if (Input::keyHold(GLFW_KEY_L)) {
				Camera::moveRight(camera_speed * delta_time);
			}
			if (Input::keyHold(GLFW_KEY_I)) {
				Camera::moveForward(camera_speed * delta_time);
			}
			if (Input::keyHold(GLFW_KEY_K)) {
				Camera::moveBack(camera_speed * delta_time);
			}
			if (Input::keyHold(GLFW_KEY_U)) {
				Camera::moveDown(camera_speed * delta_time);
			}
			if (Input::keyHold(GLFW_KEY_O)) {
				Camera::moveUp(camera_speed * delta_time);
			}
		}
	}

	void update(double delta_time) {
		if (player != nullptr) {
			updateCamera(delta_time);
			updatePlayerMovement(delta_time);
			if (first_person) {
				Camera::setPosition(player->getPos() + glm::vec3(0.0f, 0.5f, 0.0f));
			}
		}
	}
	
	void setPlayer(Player* player) {
		InputHandler::player = player;
	}
}