#include "Ray.h"

Ray::Ray(glm::vec3 pos, float max_distance, float pitch, float yaw)
	: max_distance(max_distance)
{
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = glm::normalize(direction);
	this->pos = pos;
	this->prev_pos = pos;
	cur_distance = 0.0f;
}

void Ray::step(float distance) {
	//std::cout << cur_distance << " " << max_distance << "\n";
	prev_pos = pos;
	if (cur_distance < max_distance) {
		pos += direction * distance;
		cur_distance += distance;
	}
}

glm::ivec3 Ray::blockPos() {
	return glm::round(pos);
}

pc::CardinalDirection Ray::blockFace() {
	// Assumes blockPos(prev_pos) != blockPos(pos)
	// which means blockFace() must be called after blockPos() returns
	// a solid block from somewhere else.
	glm::ivec3 direction = glm::round(prev_pos) - glm::round(pos);
	if (direction.x < 0) {
		return pc::CardinalDirection::LEFT;
	}
	else if (direction.x > 0) {
		return pc::CardinalDirection::RIGHT;
	}
	else if (direction.y < 0) {
		return pc::CardinalDirection::DOWN;
	}
	else if (direction.y > 0) {
		return pc::CardinalDirection::UP;
	}
	else if (direction.z < 0) {
		return pc::CardinalDirection::BACKWARD;
	}
	else if (direction.z > 0) {
		return pc::CardinalDirection::FORWARD;
	}
}

bool Ray::isMoving() {
	return cur_distance < max_distance;
}
