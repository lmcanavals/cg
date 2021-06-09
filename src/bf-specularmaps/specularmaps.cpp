#include <math.h>

#include <glutil.h>
#include <figures.h>
#include <camera.h>

const u32 FSIZE     = sizeof(f32);
const u32 ISIZE     = sizeof(u32);
const u32 SCRWIDTH  = 1280;
const u32 SCRHEIGHT = 720;
const f32 ASPECT    = (f32)SCRWIDTH / (f32)SCRHEIGHT;

glm::vec3 lightPos(0.0f);

Cam* cam;

f32  lastx;
f32  lasty;
f32  deltaTime  = 0.0f;
f32  lastFrame  = 0.0f;
bool firstmouse = true;
bool wireframe  = false;

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam->processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam->processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam->processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam->processKeyboard(RIGHT, deltaTime);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		wireframe = !wireframe;
	}
}

void mouse_callback(GLFWwindow* window, f64 xpos, f64 ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (firstmouse) {
			lastx = xpos;
			lasty = ypos;
			firstmouse = false;
			return;
		}
		cam->processMouse((f32)(xpos - lastx), (f32)(lasty - ypos));
		lastx = xpos;
		lasty = ypos;
	} else {
		firstmouse = true;
	}
}

void scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
	cam->processScroll((f32)yoffset);
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCRWIDTH, SCRHEIGHT, "Hola");
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	Shader* shader = new Shader("specularmaps.vert", "specularmaps.frag");
	Shader* lightCubeShader = new Shader("lightcube.vert", "lightcube.frag");
	cam = new Cam();

	Cube* cubex = new Cube();

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(0));
	glEnableVertexAttribArray(0);
	// normales: ojo que es el 3er comp, por eso offset es 6
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(6));
	glEnableVertexAttribArray(1);
	// textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(9));
	glEnableVertexAttribArray(2);

	glBindVertexArray(lightCubeVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(0));
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	shader->loadTexture("container2.png", "xyzMat.diffuse");           // tex0
	shader->loadTexture("container2_specular.png", "xyzMat.specular"); // tex1

	shader->activeTexture(0);
	shader->activeTexture(1);

	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, wireframe? GL_LINE: GL_FILL);

		glm::mat4 proj = glm::perspective(cam->getZoom(), ASPECT, 0.1f, 100.0f);

		lightPos.x = 1.5f * (cos(currentFrame) + sin(currentFrame));
		lightPos.z = 1.5f * (cos(currentFrame) - sin(currentFrame));

		shader->useProgram();

		shader->setVec3("xyz", cam->getPos());
		shader->setVec3("xyzLht.position", lightPos);
		shader->setVec3("xyzLht.ambient", 0.2f, 0.2f, 0.2f);
		shader->setVec3("xyzLht.diffuse", 0.5f, 0.5f, 0.5f);
		shader->setVec3("xyzLht.specular", 1.0f, 1.0f, 1.0f);
		shader->setF32("xyzMat.shininess", 64.0f);

		glm::mat4 model = glm::mat4(1.0f);
		shader->setMat4("proj", proj);
		shader->setMat4("view", cam->getViewM4());
		shader->setMat4("model", model);

		glBindVertexArray(cubeVao);
		glDrawElements(GL_TRIANGLES, cubex->getISize(), GL_UNSIGNED_INT, 0);

		// Luz claritaaaaaa ah ahh...
		lightCubeShader->useProgram();

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.05));
		lightCubeShader->setMat4("proj", proj);
		lightCubeShader->setMat4("view", cam->getViewM4());
		lightCubeShader->setMat4("model", model);

		glBindVertexArray(lightCubeVao);
		glDrawElements(GL_TRIANGLES, cubex->getISize(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	};

	glDeleteVertexArrays(1, &cubeVao);
	glDeleteVertexArrays(1, &lightCubeVao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete shader;
	delete lightCubeShader;
	delete cubex;
	delete cam;

	return 0;
}

