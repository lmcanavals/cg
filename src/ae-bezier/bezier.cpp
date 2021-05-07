#include <glutil.h>

const u32 FSIZE = sizeof(f32);

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, 960, 540, "Rectangulito");
	Shader* shader = new Shader(); // default: shader.vert and shader.frag

	u32 vao;
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

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

