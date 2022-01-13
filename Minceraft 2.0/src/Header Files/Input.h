#pragma once

#include "Include.h"
#include "Window.h"

namespace Input {
	bool init();
	void update();
	bool keyPress(int key);
	bool keyHold(int key);
	bool mousePress(int key);
	bool mouseHold(int key);
	glm::dvec2 getScroll();
	glm::ivec2 getMouse();
	void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseCallBack(GLFWwindow* window, int button, int action, int mods);
	void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
	void mousePosCallBack(GLFWwindow* window, double xpos, double ypos);
}