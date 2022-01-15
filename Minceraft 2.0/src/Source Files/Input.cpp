#pragma once

#include "../Header Files/Input.h"

namespace Input {

	static bool key_states[348];
	static bool prev_key_states[348];
	static bool mouse_states[5];
	static bool prev_mouse_states[5];
	static glm::dvec2 scroll;
	static int mouse_x;
	static int mouse_y;

	bool init() {
		glfwSetKeyCallback(Window::getWindow(), keyCallBack);
		glfwSetMouseButtonCallback(Window::getWindow(), mouseCallBack);
		glfwSetScrollCallback(Window::getWindow(), scrollCallBack);
		glfwSetCursorPosCallback(Window::getWindow(), mousePosCallBack);
		return true;
	}

	void update() {
		for (int i = 0; i < 348; i++) {
			prev_key_states[i] = key_states[i];
		}
		for (int i = 0; i < 3; i++) {
			prev_mouse_states[i] = mouse_states[i];
		}
	}

	bool keyPress(int key) {
		return key_states[key] && !prev_key_states[key];
	}

	bool keyHold(int key) {
		return key_states[key];
	}

	bool mousePress(int key) {
		return mouse_states[key] && !prev_mouse_states[key];
	}

	bool mouseHold(int key) {
		return mouse_states[key];
	}

	glm::dvec2 getScroll() {
		return scroll;
	}

	glm::ivec2 getMouse() {
		return glm::ivec2(mouse_x, mouse_y);
	}

	void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			key_states[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			key_states[key] = false;
		}
	}

	void mouseCallBack(GLFWwindow* window, int button, int action, int mods) {
		if (action == GLFW_PRESS) {
			mouse_states[button] = true;
		}
		else if (action == GLFW_RELEASE) {
			mouse_states[button] = false;
		}
	}

	void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
		scroll.x = xoffset;
		scroll.y = yoffset;
	}

	void Input::mousePosCallBack(GLFWwindow* window, double xpos, double ypos) {
		mouse_x = xpos;
		mouse_y = ypos;
	}
}