#include <glutil.h>

const unsigned int FSIZE = sizeof(float);

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	GLFWwindow* window = glutilInit(3, 3, 960, 540, "Rectangulito");
	Shader* shader = new Shader(); // default: shader.vert and shader.frag

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.1, 0.2, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->useProgram();
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, 100);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);

	delete shader;

	return 0;

}
