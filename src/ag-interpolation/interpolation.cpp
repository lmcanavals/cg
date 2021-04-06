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

	float vertices[] = {
		 /* posiciones */   /* colores */
		 0.5,  0.5,  0.0,   1.0, 0.0, 0.0,
		 0.5, -0.5,  0.0,   0.0, 1.0, 0.0,
		-0.5, -0.5,  0.0,   1.0, 0.5, 0.0,
		-0.5,  0.5,  0.0,   0.0, 0.0, 1.0 };
	unsigned int idxs[] = {
		0, 1, 3,
		1, 2, 3 };

	unsigned int vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxs), idxs, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*FSIZE, (void*)(3*FSIZE));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.1, 0.2, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->useProgram();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete shader;

	return 0;
}
