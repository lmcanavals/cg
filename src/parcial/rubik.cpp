#include <glutil.h>
#include <figures.h>
#include <camera.h>

#include <math.h>

#include "rubik.h"

const u32 FSIZE      = sizeof(f32);
const u32 ISIZE      = sizeof(i32);
const u32 SCR_WIDTH  = 1280;
const u32 SCR_HEIGHT = 720;
const f32 ASPECT     = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

glm::vec3 lightPos(1.5f, 1.5f, 2.5f);

Cam cam(1.0f, 2.5f, 5.0f);
Rubik* rubik = new Rubik();

f32  lastx;
f32  lasty;
bool firstMouse = true;
f32  deltaTime  = 0.0f;
f32  lastFrame  = 0.0f;

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.processKeyboard(RIGHT, deltaTime);
	}
}

void key_callback(GLFWwindow* window, int key, int code, int action, int mods) {
	if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		rubik->rotateX(0);
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		rubik->rotateX(1);
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		rubik->rotateX(2);
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		rubik->rotateY(0);
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		rubik->rotateY(1);
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		rubik->rotateY(2);
	}
}

void mouse_callback(GLFWwindow* window, f64 xpos, f64 ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (firstMouse) {
			lastx = xpos;
			lasty = ypos;
			firstMouse = false;
			return;
		}
		cam.processMouse((f32)(xpos - lastx), (f32)(lasty - ypos));
		lastx = xpos;
		lasty = ypos;
	} else {
		firstMouse = true;
	}
}

void scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
	cam.processScroll((f32)yoffset);
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Bleep!");
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	Shader* lightingShader = new Shader();
	Shader* lightCubeShader = new Shader("bin", "resources/textures",
			"shader.vert", "shader2.frag");

	Cube* cubex = new Cube(0.99f, 0.99f, 0.99f);

	u32 cubeVao, lightCubeVao, vbo, ebo;
	glGenVertexArrays(1, &cubeVao);
	glGenVertexArrays(1, &lightCubeVao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(cubeVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, cubex->getVSize()*FSIZE,
			cubex->getVertices(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubex->getISize()*ISIZE,
			cubex->getIndices(), GL_STATIC_DRAW);

	// posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(3*FSIZE));
	glEnableVertexAttribArray(1);

	glBindVertexArray(lightCubeVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 proj = glm::perspective(cam.getZoom(), ASPECT, 0.1f, 100.0f);

		glBindVertexArray(cubeVao);
		lightingShader->useProgram();
		lightingShader->setMat4("proj", proj);
		lightingShader->setMat4("view", cam.getViewM4());

		u32 i, j, k;
		for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j) for (k = 0; k < 3; ++k) {
			lightingShader->setMat4("model", rubik->getModel(i, j, k));
			glDrawElements(GL_TRIANGLES, cubex->getISize(), GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(lightCubeVao);
		lightCubeShader->useProgram();
		lightCubeShader->setMat4("proj", proj);
		lightCubeShader->setMat4("view", cam.getViewM4());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader->setMat4("model", model);
		glDrawElements(GL_TRIANGLES, cubex->getISize(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &cubeVao);
	glDeleteVertexArrays(1, &lightCubeVao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete lightingShader;
	delete lightCubeShader;
	delete cubex;

	return 0;
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

