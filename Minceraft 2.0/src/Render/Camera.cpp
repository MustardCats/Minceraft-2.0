#include "Camera.h"

namespace Camera {

	static glm::vec3 pos;
	static double yaw;
	static double pitch;
	// move to settings eventually
	static float fov;

	bool init() {
		pos = glm::vec3(0.5, 0.5, 0.5);
		fov = 90.0f;
		yaw = 90.0f;
		pitch = 0.0f;

		return true;
	}

	glm::mat4 getMatrix() {
		return getProjectionMatrix() * getViewMatrix();
	}

	glm::mat4 getProjectionMatrix() {
		return glm::perspective(glm::radians(fov), (-(float)Window::getSize().x) / ((float)Window::getSize().y), 0.1f, 1000.0f);
	}

	glm::mat4 getViewMatrix() {
		glm::vec3 direction = getDirection();
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		return glm::lookAt(pos, pos + direction, up);
	}

	glm::vec3 getDirection() {
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction = glm::normalize(direction);
		return direction;
	}
	
	glm::vec3 getRight() {
		glm::vec3 right = glm::vec3(
			cos(glm::radians(yaw) - 3.14f / 2),
			0,
			sin(glm::radians(yaw) - 3.14f / 2)
		);

		return right;
	}

	glm::vec3 getForward() {
		glm::vec3 forward = glm::vec3(
			cos(glm::radians(yaw)),
			0,
			sin(glm::radians(yaw))
		);

		return forward;
	}

	void moveForward(float distance) {
		pos += getDirection() * distance;
	}

	void moveBack(float distance) {
		pos -= getDirection() * distance;
	}

	void moveLeft(float distance) {
		pos -= getRight() * distance;
	}

	void moveRight(float distance) {
		pos += getRight() * distance;
	}

	void strafeForward(float distance) {
		glm::vec3 direction = getDirection();
		direction.y = 0.0f;
		pos += direction * distance;
	}

	void strafeBack(float distance) {
		glm::vec3 direction = getDirection();
		direction.y = 0.0f;
		pos -= direction * distance;
	}

	void moveUp(float distance) {
		pos.y += distance;
	}

	void moveDown(float distance) {
		pos.y -= distance;
	}

	void addYaw(float distance) {
		yaw += distance;
		yaw = glm::mod((float)yaw, 360.0f);
	}

	void addPitch(float distance) {
		pitch += distance;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	float getYaw() {
		return yaw;
	}

	float getPitch() {
		return pitch;
	}

	void setPosition(glm::vec3 position) {
		pos = position;
	}

	void setFOV(float fov) {
		Camera::fov = fov;
	}
}