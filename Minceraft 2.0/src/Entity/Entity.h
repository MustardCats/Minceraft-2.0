#pragma once

#include "../Include.h"
#include "../Physics/Shape.h"
#include "../World/ChunkManager.h"

class Entity {
public:
	Entity();
	void update(float delta_time);
	void addPos(glm::vec3 offset);
	void move(glm::vec3 direction);
	void jump();
	void run();
	glm::vec3& getPos();
	glm::vec3 getSize();
	glm::vec3 getVelocity();
	Rectangle rigid_body;
protected:
	void checkCollision(glm::vec3 distance);
	glm::vec3 pos;
	glm::vec3 size;
	glm::vec3 velocity;
	glm::vec3 direction;
	bool input_jump;
	bool running;
	float max_speed;
};