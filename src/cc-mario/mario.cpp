#include <files.hpp>
#include <model.hpp>
#include <cam.hpp>

const u32 SCR_WIDTH  = 1280;
const u32 SCR_HEIGHT = 720;
const f32 ASPECT     = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

Cam* cam;

f32  deltaTime  = 0.0f;
f32  lastFrame  = 0.0f;
bool wireframe  = false;

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
void key_callback(GLFWwindow*, int key, int, int act, int) {
	wireframe ^= key == GLFW_KEY_E && act == GLFW_PRESS;
}
void mouse_callback(GLFWwindow* window, f64 xpos, f64 ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		cam->movePov(xpos, ypos);
	} else {
		cam->stopPov();
	}
}
void scroll_callback(GLFWwindow*, f64, f64 yoffset) {
	cam->processScroll((f32)yoffset);
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Cubito");
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	cam = new Cam({0.0f, 1.0f, 2.5f});
	cam->speed = 0.75f;

	Files* files = new Files("bin", "resources/textures", "resources/objects");

	Shader* shader     = new Shader(files, "shader.vert", "shader.frag");
	Shader* moniShader = new Shader(files, "monito.vert", "monito.frag");
	Model*  marioball  = new Model(files, "marioball/marioball.obj");
	Model*  monito     = new Model(files, "monito/monito.obj");

	u32 amount = 2500;
	glm::mat4* models = new glm::mat4[amount];
	srand(glfwGetTime());
	f32 radius = 2.5f;
	auto displacement = [radius](){return rand()%100000/1e5f * radius * 2.0f;};
	for (u32 i = 0; i < amount; ++i) {
		glm::mat4 model = glm::mat4(1.0f);
		f32 angle = (f32)i / (f32)amount * 360.0f;
		f32 x = sin(angle) * (radius + displacement());
		f32 y = displacement() * 0.15f;
		f32 z = cos(angle) * (radius + displacement());
		model = glm::translate(model, {x, y, z});

		f32 scale = rand() % 10 / 1000.0f + 0.005;
		model = glm::scale(model, glm::vec3(scale));

		f32 rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, {0.4f, 0.6f, 0.8f});

		models[i] = model;
	}
	u32 buffer;
	const u32 smat4 = sizeof(glm::mat4);
	const u32 svec4 = sizeof(glm::vec4);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * smat4, &models[0], GL_STATIC_DRAW);

	for (u32 i = 0; i < monito->meshes.size(); ++i) {
		glBindVertexArray(monito->meshes[i].Vao);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, smat4, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, smat4, (void*)svec4);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, smat4, (void*)(2*svec4));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, smat4, (void*)(3*svec4));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	glm::vec3 lightPos   = glm::vec3(1.0f);
	glm::vec3 lightColor = glm::vec3(1.0f);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, wireframe? GL_LINE: GL_FILL);

		shader->use();
		lightPos.x = radius*(cos(currentFrame) - sin(currentFrame));
		lightPos.z = radius*(cos(currentFrame) + sin(currentFrame));
		shader->setVec3("xyz", lightPos);
		shader->setVec3("xyzColor", lightColor);
		shader->setVec3("xyzView", cam->pos);
		glm::mat4 proj = glm::perspective(cam->zoom, ASPECT, 0.1f, 1000.0f);
		shader->setMat4("proj", proj);
		shader->setMat4("view", cam->getViewM4());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(radius / 2.0f));
		shader->setMat4("model", model);
		marioball->Draw(shader);

		moniShader->use();
		moniShader->setMat4("proj", proj);
		moniShader->setMat4("view", cam->getViewM4());
		moniShader->setI32("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, monito->textures_loaded[0].id);
		for (u32 i = 0; i < monito->meshes.size(); ++i) {
			glBindVertexArray(monito->meshes[i].Vao);
			glDrawElementsInstanced(GL_TRIANGLES,	monito->meshes[i].indices.size(),
				GL_UNSIGNED_INT, 0, amount);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete cam;
	delete shader;
	delete marioball;
	delete[] models;

	return 0;
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

