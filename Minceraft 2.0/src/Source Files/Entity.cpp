#include "../Header Files/Entity.h"

Entity::Entity() {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	size = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Entity::addPos(glm::vec3 offset) {
	pos += offset;
}

glm::vec3 Entity::getPos() {
	return pos;
}

glm::vec3 Entity::getSize() {
	return size;
}

glm::vec3 Entity::getVelocity() {
	return glm::vec3();
}

void Entity::checkCollision() {

}