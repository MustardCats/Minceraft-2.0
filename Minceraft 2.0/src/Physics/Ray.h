#pragma once

#include "../Include.h"
#include "../Constants.h"

class Ray {
public:
	Ray(glm::vec3 pos, float max_distance, float pitch, float yaw);
	void step(float distance);
	glm::ivec3 blockPos();
	pc::CardinalDirection blockFace();
	bool isMoving();
private:
	glm::vec3 pos;
	glm::vec3 prev_pos;
	float cur_distance;
	glm::vec3 direction;
	const float max_distance;
};