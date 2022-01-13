#pragma once

#include "Include.h"

namespace Window {
	bool init();
	bool update();
	void setWindowTitle(std::string title);
	void lockMouse();
	void unlockMouse();
	void frameBuffSizeCallback(GLFWwindow* window, int width, int height);
	glm::ivec2 getSize();
	GLFWwindow* getWindow();
}