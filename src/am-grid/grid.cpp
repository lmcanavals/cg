#include <glutil.h>

#include <math.h>

const u32 FSIZE = sizeof(f32);
const u32 ISIZE = sizeof(i32);
const u32 SCR_WIDTH = 960;
const u32 SCR_HEIGHT = 540;
const f32  ASPECT = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

f32* makeVertices(u32 xblocks, u32 yblocks, u32 comps,
	                f32 from, f32 to) {
	f32* vertices = new f32[xblocks * yblocks * comps];
	f32 incx = (to - from) / (xblocks-1);
	f32 incy = (to - from) / (yblocks-1);
	f32* c = new f32[comps];
	for (u32 i = 0; i < comps; ++i) {
		c[i] = 0.5f;
	}
	c[2] = to;                             // z component
	for (u32 x = 0; x < xblocks; ++x) {
		c[0] = from;                         // x component
		u32 offx = x*yblocks*comps;
		for (u32 y = 0; y < yblocks; ++y) {
			c[1] = sin(c[0]*c[2]*8.0f) * 0.5;  // y component
			c[3] = 1.0f - c[1];
			u32 offy = offx + y * comps;
			for (u32 k = 0; k < comps; ++k) {
				vertices[offy + k] = c[k];
			}
			c[0] += incy;
		}
		c[2] -=  incx;
	}
	return vertices;
}

u32* makeIdxs(u32 xblocks, u32 yblocks) {
	u32* idxs = new u32[(xblocks-1)*(yblocks-1)*6];

	for (u32 x = 1; x < xblocks; ++x) {
		u32 offx = (x-1) * (yblocks - 1) * 6;
		for (u32 y = 1; y < yblocks; ++y) {
			u32 offy = offx + (y-1) * 6;
			idxs[offy + 0] = (x-1) * yblocks + y - 1;
			idxs[offy + 1] = (x-1) * yblocks + y;
			idxs[offy + 2] = x * yblocks + y - 1;
			idxs[offy + 3] = (x-1) * yblocks + y;
			idxs[offy + 4] = x * yblocks + y - 1;
			idxs[offy + 5] = x * yblocks + y;
		}
	}
	return idxs;
}

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Terrain?");
	Shader* shader = new Shader();

	u32 xblocks = 100, yblocks = 100, comps = 6;
	f32 from = -1.0f, to = 1.0f;

	f32* vertices = makeVertices(xblocks, yblocks, comps, from, to);
	u32* idxs    = makeIdxs(xblocks, yblocks);

	u32 vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	u32 vsize = FSIZE*(xblocks*yblocks*comps);
	glBufferData(GL_ARRAY_BUFFER, vsize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	u32 isize = ISIZE*(xblocks-1)*(yblocks-1)*6;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, idxs, GL_STATIC_DRAW);

	// posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);
	// colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*FSIZE, (void*)(3*FSIZE));
	glEnableVertexAttribArray(1);
	// coordenadas de textura
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(6*FSIZE));
	//glEnableVertexAttribArray(2);

	//u32 texture = shader->loadTexture("mafalda.jpg");

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, texture);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->useProgram();

		glm::mat4 model      = glm::mat4(1.0f);
		glm::mat4 view       = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		f32 theta = (f32)glfwGetTime();
		model      = glm::rotate(model, theta, glm::vec3(0.00f, 1.0f, 0.0f));
		view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
		projection = glm::perspective(glm::radians(-45.0f), ASPECT, 0.1f, 100.0f);

		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("proj", projection);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, (xblocks-1)*(yblocks-1)*6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete shader;
	delete[] vertices;
	delete[] idxs;

	return 0;
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

