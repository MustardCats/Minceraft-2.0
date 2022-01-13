#pragma once

#include "Include.h"

class Entity {
public:
	Entity();
	void addPos(glm::vec3 offset);
	glm::vec3 getPos();
	glm::vec3 getSize();
	glm::vec3 getVelocity();
protected:
	void checkCollision();
	glm::vec3 pos;;
	glm::vec3 size;
	glm::vec3 velocity;
};