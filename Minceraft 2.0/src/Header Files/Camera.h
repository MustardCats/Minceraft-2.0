#pragma once

#include "Include.h"
#include "Window.h"

namespace Camera {
	bool init();
	glm::mat4 getMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::vec3 getDirection();
	glm::vec3 getRight();
	glm::vec3 getForward();
	void moveForward(float distance);
	void moveBack(float distance);
	void moveLeft(float distance);
	void moveRight(float distance);
	void strafeForward(float distance);
	void strafeBack(float distance);
	void moveUp(float distance);
	void moveDown(float distance);
	void addYaw(float distance);
	void addPitch(float distance);
	void setPosition(glm::vec3 position);
	void setFOV(float fov);
}