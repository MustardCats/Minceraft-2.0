#include "Window.h"

namespace Window {

	static GLFWwindow* window = nullptr;
	static glm::ivec2 size = glm::ivec2();
	static bool mouse_locked;
	// update immediately when locking/unlocking to
	// prevent suddden camera movement
	static glm::ivec2* mouse_pos;
	static glm::ivec2* prev_mouse_pos;

	bool Window::init() {
		if (!glfwInit()) {
			return false;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(1024, 768, "fuck", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);
		glfwSetFramebufferSizeCallback(window, frameBuffSizeCallback);

		gladLoadGL();

		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, 1024, 768);
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

		mouse_locked = false;

		return true;
	}

	glm::ivec2 getSize() {
		return size;
	}

	bool update() {
		glfwPollEvents();
		glfwGetWindowSize(window, &size.x, &size.y);

		if (glfwWindowShouldClose(window)) {
			glfwDestroyWindow(window);
			return false;
		}
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LEQUAL);

		return true;
	}

	void setWindowTitle(std::string title) {
		glfwSetWindowTitle(window, title.c_str());
	}

	void lockMouse() {
		// Btw setting cursor pos with this will NOT update the 
		// mouse_pos in input automatically, like how moving the mouse does.
		// This caused an issue of the camera "jumping" when moving 
		// the mouse right after re-locking the mouse.
		glfwSetCursorPos(window, size.x / 2, size.y / 2);
		if (mouse_pos && prev_mouse_pos) {
			*mouse_pos = *prev_mouse_pos = size / 2;
		}
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouse_locked = true;
	}

	void unlockMouse() {
		glfwSetCursorPos(window, size.x / 2, size.y / 2);
		if (mouse_pos && prev_mouse_pos) {
			*mouse_pos = *prev_mouse_pos = size / 2;
		}
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouse_locked = false;
	}

	bool isMouseLocked() {
		return mouse_locked;
	}

	void setMousePos(glm::ivec2* mouse_pos, glm::ivec2* prev_mouse_pos) {
		Window::mouse_pos = mouse_pos;
		Window::prev_mouse_pos = prev_mouse_pos;
	}

	GLFWwindow* getWindow() {
		return window;
	}

	void frameBuffSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
}