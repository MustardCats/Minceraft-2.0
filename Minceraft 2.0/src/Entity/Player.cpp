#include "Player.h"

Player::Player() : inv(36), cursor_item(0, 0), Entity() {
	size = glm::vec3(0.75f, 1.8f, 0.75f);
	pos.x = 17;
	pos.y = 50;
	pos.z = 17;
	ray = nullptr;
	facing_block = false;
	inside_block = false;
	block_pos = glm::ivec3(0);
	inv_open = true;
	rigid_body.changeSize(size);
	// Test items
	for (int i = 1; i < 4; i++) {
		inv.add(Item(i, 100));
	}
}

Player::~Player() {
	
}

void Player::update(float delta_time) {
	Entity::update(delta_time);
	if (ray != nullptr) {
		delete ray;
	}
	ray = new Ray(pos, 5.0f, Camera::getPitch(), Camera::getYaw());
	glm::ivec3 initial_pos = ray->blockPos();
	Block* block = ChunkManager::getBlock(initial_pos);
	inside_block = false;
	if (block != nullptr && block->id != 0) {
		inside_block = true;
	}
	while (ray->isMoving()) {
		ray->step(0.1f);
		glm::ivec3 pos = ray->blockPos();
		Block* block = ChunkManager::getBlock(pos);
		if (block != nullptr && block->id != 0) {
			facing_block = true;
			block_pos = pos;
			break;
		}
	}
}

void Player::tryBreakBlock() {
	if (facing_block && !inv_open) {
		ChunkManager::changeBlock(block_pos, 0);
	}
}

void Player::tryPlaceBlock() {
	if (facing_block && !inside_block && !inv_open) {
		switch (ray->blockFace()) {
		case pc::CardinalDirection::LEFT:
			ChunkManager::changeBlock(block_pos + glm::ivec3(-1, 0, 0), 3);
			break;
		case pc::CardinalDirection::RIGHT:
			ChunkManager::changeBlock(block_pos + glm::ivec3(1, 0, 0), 3);
			break;
		case pc::CardinalDirection::UP:
			ChunkManager::changeBlock(block_pos + glm::ivec3(0, 1, 0), 3);
			break;
		case pc::CardinalDirection::DOWN:
			ChunkManager::changeBlock(block_pos + glm::ivec3(0, -1, 0), 3);
			break;
		case pc::CardinalDirection::FORWARD:
			ChunkManager::changeBlock(block_pos + glm::ivec3(0, 0, 1), 3);
			break;
		case pc::CardinalDirection::BACKWARD:
			ChunkManager::changeBlock(block_pos + glm::ivec3(0, 0, -1), 3);
			break;
		}
	}
}

Inventory* Player::getInventory() {
	return &inv;
}

bool Player::isInvOpen() {
	return inv_open;
}

void Player::toggleInv() {
	inv_open = 1 - inv_open;
}

void Player::leftClickInv(glm::ivec2 mouse) {
	if (inv_open) {

	}
}

void Player::rightClickInv(glm::ivec2 mouse) {
	if (inv_open) {

	}
}

