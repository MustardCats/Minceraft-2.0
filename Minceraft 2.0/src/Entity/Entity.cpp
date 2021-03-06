#include "Entity.h"

Entity::Entity() : rigid_body(glm::vec3(0.0f), glm::vec3(0.0f)) {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	size = glm::vec3(1.0f, 1.0f, 1.0f);
	direction = glm::vec3(0.0f);
	velocity = glm::vec3(0.0f);
	rigid_body.changePos(pos);
	rigid_body.changeSize(size);
	input_jump = false;
	running = false;
}

void Entity::update(float delta_time) {
	const float INITIAL_SPEED = 5.0f;
	const float INITIAL_JUMP = 10.0f;
	const float ACCELERATION = 10.0f;
	const float GRAVITY = 9.8f;

	if (running) {
		max_speed = 100.0f;
	}
	else {
		max_speed = 10.0f;
	}

	float velocity_distance = glm::distance(glm::vec3(0.0f), velocity);
	if (direction != glm::vec3(0.0f)) {
		if (velocity == glm::vec3(0.0f)) {
			velocity = direction * INITIAL_SPEED;
		}
		else {
			velocity = velocity_distance * direction;
			velocity += direction * ACCELERATION;
		}
	}
	else {
		velocity = glm::vec3(0.0f);
	}
	velocity_distance = glm::distance(glm::vec3(0.0f), velocity);
	if (velocity_distance > max_speed) {
		velocity = glm::normalize(velocity) * max_speed;
	}

	checkCollision(velocity * delta_time);

	direction = glm::vec3(0.0f);
	input_jump = false;
	running = false;
}

void Entity::addPos(glm::vec3 offset) {
	pos += offset;
}

void Entity::jump() {
	input_jump = true;
}

glm::vec3& Entity::getPos() {
	return pos;
}

void Entity::run() {
	running = true;
}

glm::vec3 Entity::getSize() {
	return size;
}

glm::vec3 Entity::getVelocity() {
	return glm::vec3();
}

void Entity::move(glm::vec3 direction) {
	this->direction += direction;
}

void Entity::checkCollision(glm::vec3 distance) {
	glm::vec3 new_pos = pos + distance;
	rigid_body.changePos(new_pos);
	std::vector<Rectangle> cube_hitboxes;
	glm::ivec3 start = glm::round(new_pos - (size / 2.0f));
	glm::ivec3 end = glm::round(new_pos + (size / 2.0f));
	glm::ivec3 index;
	for (index.x = start.x; index.x <= end.x; index.x++) {
		for (index.y = start.y; index.y <= end.y; index.y++) {
			for (index.z = start.z; index.z <= end.z; index.z++) {
				Block* block = ChunkManager::getBlock(index);
				if (block != nullptr && block->id > 0) {
					cube_hitboxes.push_back(Rectangle(index));
				}
			}
		}
	}
	if (cube_hitboxes.size() == 0) {
		pos = new_pos;
		return;
	}
	new_pos -= distance;
	
	// X
	new_pos.x += distance.x;
	rigid_body.changePos(new_pos);
	for (const Rectangle& rect : cube_hitboxes) {
		if (rigid_body.rectIntersects(rect)) {
			new_pos.x = pos.x;
			break;
		}
	}
	// Y
	new_pos.y += distance.y;
	rigid_body.changePos(new_pos);
	for (const Rectangle& rect : cube_hitboxes) {
		if (rigid_body.rectIntersects(rect)) {
			new_pos.y = pos.y;
			break;
		}
	}
	// Z
	new_pos.z += distance.z;
	rigid_body.changePos(new_pos);
	for (const Rectangle& rect : cube_hitboxes) {
		if (rigid_body.rectIntersects(rect)) {
			new_pos.z = pos.z;
			break;
		}
	}
	
	pos = new_pos;
}