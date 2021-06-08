#include <glutil.h>
#include <figures.h>

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const u32 FSIZE      = sizeof(f32);
const u32 ISIZE      = sizeof(i32);
const u32 SCR_WIDTH  = 1280;
const u32 SCR_HEIGHT = 720;
const f32  ASPECT     = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

glm::vec3 position = glm::vec3(2.0f, 1.0f, 6.0f);
glm::vec3 front    = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
f32 yaw   = -90.0f;
f32 pitch = 0.0f;
f32 lastx = SCR_WIDTH / 2.0f;
f32 lasty = SCR_HEIGHT / 2.0f;
f32 fov   = 45.0f;

f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	f32 speed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += speed * front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= glm::normalize(glm::cross(front, up)) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= speed * front;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += glm::normalize(glm::cross(front, up)) * speed;
	}
}

void mouse_callback(GLFWwindow*, f64 xpos, f64 ypos) {
	if (firstMouse) {
		lastx = xpos;
		lasty = ypos;
		firstMouse = false;
	}
	f32 xoffset = xpos - lastx;
	f32 yoffset = lasty - ypos;

	lastx = xpos;
	lasty = ypos;

	f32 sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw   += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 f;
	f.x   = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y   = sin(glm::radians(pitch));
	f.z   = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(f);
}

void scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
	fov -= (f32)yoffset; // zoom
	if (fov < 1.0f) {
		fov = 1.0f;
	} else if (fov > 45.0f) {
		fov = 45.0f;
	}
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Terrain?");
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	Shader* lightingShader = new Shader();
	Shader* lightCubeShader = new Shader("shader.vert", "shader2.frag");

	Cube* cubex = new Cube();

	u32 n = 1;
	std::vector<glm::vec3> positions(n*n);
	for (u32 i = 0; i < n; ++i) {
		for (u32 j = 0; j < n; ++j) {
			f32 x = i - n/2.0f;
			f32 z = j - n/2.0f;
			f32 y = 0.0f;// x*x / 10 - z*z / 10;
			positions[i*n+j] = glm::vec3(x, y, z);
		}
	}

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(lightCubeVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//glBindTexture(GL_TEXTURE_2D, texture1);

		processInput(window);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		lightingShader->useProgram();
		lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(fov), ASPECT, 0.1f, 100.0f);
		lightingShader->setMat4("proj", projection);

		glm::mat4 view       = glm::mat4(1.0f);
		view  = glm::lookAt(position, position + front, up);
		lightingShader->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		lightingShader->setMat4("model", model);

		glBindVertexArray(cubeVao);
		glDrawElements(GL_TRIANGLES, cubex->getISize(), GL_UNSIGNED_INT, 0);

		lightCubeShader->useProgram();
		lightCubeShader->setMat4("proj", projection);
		lightCubeShader->setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader->setMat4("model", model);

		glBindVertexArray(lightCubeVao);
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

