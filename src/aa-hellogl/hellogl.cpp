#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	if (!glfwInit()) {
		std::cerr << "Can't start glfw :(\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, ":)", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Can't create window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Can't load GLAD\n";
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
