#include "../Header Files/Window.h"

namespace Window {

	static GLFWwindow* window = nullptr;
	static glm::ivec2 size = glm::ivec2();

	bool Window::init() {
		if (!glfwInit()) {
			return false;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(600, 400, "fuck", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);
		glfwSetFramebufferSizeCallback(window, frameBuffSizeCallback);

		gladLoadGL();

		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, 600, 400);
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		//glClearDepth(1.0f);
		//glDepthMask(GL_FALSE);
		

		lockMouse();

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
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void unlockMouse() {
		
	}

	GLFWwindow* getWindow() {
		return window;
	}

	void frameBuffSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
}