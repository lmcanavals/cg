#include <glutil.h>

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const u32 FSIZE      = sizeof(f32);
const u32 ISIZE      = sizeof(i32);
const u32 SCR_WIDTH  = 1280;
const u32 SCR_HEIGHT = 720;
const f32  ASPECT     = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

glm::vec3 position = glm::vec3(0.0f, 3.0f, 4.0f);
glm::vec3 front    = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
f32 yaw   = -90.0f;
f32 pitch = 0.0f;
f32 lastx = SCR_WIDTH / 2.0f;
f32 lasty = SCR_HEIGHT / 2.0f;
f32 fov   = 45.0f;

f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;

class Cube {

	f32   width;
	f32   height;
	f32   depth;
	f32*  vertices;
	u32* indices;

public:
	Cube(f32 width=1.0f, f32 height=1.0f, f32 depth=1.0f)
			: width(width), height(height), depth(depth),
			  vertices(new f32[16*8]), indices(new u32[6*6]) {
		f32 wm = width / 2.0f;
		f32 hm = height / 2.0f;
		f32 dm = depth / 2.0f;
		f32 temp[] = {
			 // posiciones        colores          texturas
			-wm,  hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 0
			 wm,  hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 1
			-wm, -hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 2
			 wm, -hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 3
			-wm,  hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 4
			 wm,  hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 5
			-wm, -hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 6
			 wm, -hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 7

			-wm,  hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 8
			 wm,  hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 9
			-wm, -hm,  dm,   1.0f, 0.0, 0.0,   0.0f, 0.0f,  // 10
			 wm, -hm,  dm,   1.0f, 0.0, 0.0,   1.0f, 0.0f,  // 11
			-wm,  hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 12
			 wm,  hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f,  // 13
			-wm, -hm, -dm,   1.0f, 0.0, 0.0,   0.0f, 1.0f,  // 14
			 wm, -hm, -dm,   1.0f, 0.0, 0.0,   1.0f, 1.0f}; // 15
		for (u32 i = 0; i < 16*8; ++i) {
			vertices[i] = temp[i];
		}
		u32 temp2[] = {
			 0,  1,  2,   1,  2,  3,
			 8,  9, 12,   9, 12, 13,
			 1,  5,  3,   3,  5,  7,
			11, 14, 15,  10, 11, 14,
			 0,  4,  6,   0,  2,  6,
			 4,  5,  6,   5,  6,  7};
		for (u32 i = 0; i < 6*6; ++i) {
			indices[i] = temp2[i];
		}
	}
	~Cube() {
		delete[] vertices;
		delete[] indices;
	}
	f32* getVertices() {
		return vertices;
	}
	u32* getIndices() {
		return indices;
	}
	u32 getVSize() {
		return 16*8;
	}
	u32 getISize() {
		return 6*6;
	}
};

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
	Shader* shader = new Shader();

	Cube* cubex = new Cube();

	srand(time(0));
	u32 n = 100;
	std::vector<glm::vec3> positions(n*n);
	for (u32 i = 0; i < n; ++i) {
		for (u32 j = 0; j < n; ++j) {
			f32 x = i - n/2.0f;
			f32 z = j - n/2.0f;
			f32 y = 0.0f;// x*x / 10 - z*z / 10;
			positions[i*n+j] = glm::vec3(x, y, z);
		}
	}

	u32 vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, cubex->getVSize()*FSIZE,
	             cubex->getVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubex->getISize()*ISIZE,
	             cubex->getIndices(), GL_STATIC_DRAW);

	// posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);
	// colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(3*FSIZE));
	glEnableVertexAttribArray(1);
	// textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(6*FSIZE));
	glEnableVertexAttribArray(2);
	
	u32 texture1 = shader->loadTexture("container.jpg");

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glBindTexture(GL_TEXTURE_2D, texture1);

		processInput(window);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->useProgram();

		glm::mat4 projection = glm::perspective(glm::radians(fov), ASPECT, 0.1f, 100.0f);
		shader->setMat4("proj", projection);

		glm::mat4 view       = glm::mat4(1.0f);
		view  = glm::lookAt(position, position + front, up);
		shader->setMat4("view", view);

		glBindVertexArray(vao);
		for (u32 i = 0; i < positions.size(); ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);
			//f32 theta = (f32)glfwGetTime();
			//model = glm::rotate(model, theta, glm::vec3(0.3f, 0.6f, 0.9f));
			shader->setMat4("model", model);

			glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete shader;
	delete cubex;

	return 0;
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

