#include <glutil.h>

using namespace glm;
using namespace std;

const u32 FSIZE = sizeof(f32);
const u32 SCR_WIDTH = 960;
const u32 SCR_HEIGHT = 540;
const f32  ASPECT = 16.0f / 9.0f;

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Cubito");
	Shader* shader = new Shader("bin", "resources/textures");

	glEnable(GL_DEPTH_TEST);
	f32 vertices[] = {
		 // posiciones        colores          texturas
		-0.5f,  0.5f,  0.5f,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 0
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 1
		-0.5f, -0.5f,  0.5f,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 2
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 3
		-0.5f,  0.5f, -0.5f,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 4
		 0.5f,  0.5f, -0.5f,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 5
		-0.5f, -0.5f, -0.5f,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 6
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 7

		-0.5f,  0.5f,  0.5f,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 8
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 9
		-0.5f, -0.5f,  0.5f,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 10
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 11
		-0.5f,  0.5f, -0.5f,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 12
		 0.5f,  0.5f, -0.5f,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 13
		-0.5f, -0.5f, -0.5f,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 14
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0, 0.0,   1.0f, 1.0f}; // 15
	u32 idxs[] = {
		 0,  1,  2,   1,  2,  3,
		 8,  9, 12,   9, 12, 13,
		 1,  5,  3,   3,  5,  7,
		11, 14, 15,  10, 11, 14,
		 0,  4,  6,   0,  2,  6,
		 4,  5,  6,   5,  6,  7};

	u32 vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxs), idxs, GL_STATIC_DRAW);

	// posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);
	// colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(3*FSIZE));
	glEnableVertexAttribArray(1);
	// coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(6*FSIZE));
	glEnableVertexAttribArray(2);

	u32 texture1 = shader->loadTexture("mafalda.jpg");
	u32 texture2 = shader->loadTexture("container.jpg");

	glBindVertexArray(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.1, 0.2, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture2);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->useProgram();

		mat4 model      = mat4(1.0);
		mat4 view       = mat4(1.0);
		mat4 projection = mat4(1.0);
		f32 theta = (f32)glfwGetTime();
		model = rotate(model, theta, vec3(0.3, 0.6, 1.0));
		view  = translate(view, vec3(0.0, 0.0, -6.0));
		projection = perspective(radians(45.0f), ASPECT, 0.1f, 100.0f);

		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("proj", projection);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete shader;

	return 0;
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

