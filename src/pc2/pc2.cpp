#include <math.h>
#include <stdlib.h>

#include <glutil.h>
#include <figures.h>
#include <camera.h>

#include <maze.h>

const u32 FSIZE     = sizeof(f32);
const u32 ISIZE     = sizeof(u32);
const u32 SCRWIDTH  = 1280;
const u32 SCRHEIGHT = 720;
const f32 ASPECT    = (f32)SCRWIDTH / (f32)SCRHEIGHT;

Cam* cam;

f32  lastx;
f32  lasty;
f32  deltaTime  = 0.0f;
f32  lastFrame  = 0.0f;
bool firstmouse = true;
bool wireframe  = false;

class PointLight {
	Shader* shader;

	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	f32 constant;
	f32 linear;
	f32 quadratic;

	std::string positionName;
	std::string ambientName;
	std::string diffuseName;
	std::string specularName;
	std::string constantName;
	std::string linearName;
	std::string quadraticName;
public:
	PointLight(Shader* shader, std::string uniformName, glm::vec3 position,
	           glm::vec3 ambient = glm::vec3(0.05f),
	           glm::vec3 diffuse = glm::vec3(0.8f),
	           glm::vec3 specular = glm::vec3(1.0f),
	           f32 constant = 1.0f, f32 linear = 0.09f, f32 quadratic = 0.032f)
			:	shader(shader),
				position(position),
				ambient(ambient),
				diffuse(diffuse),
				specular(specular),
				constant(constant),
				linear(linear),
				quadratic(quadratic),
				positionName(uniformName  + ".position"),
				ambientName(uniformName   + ".ambient"),
				diffuseName(uniformName   + ".diffuse"),
				specularName(uniformName  + ".specular"),
				constantName(uniformName  + ".constant"),
				linearName(uniformName    + ".linear"),
				quadraticName(uniformName + ".quadratic") {}

	glm::vec3& getPosition() {
		return position;
	}
	void setPosition(glm::vec3 newpos) {
		position = newpos;
	}
void setUniforms() {
		shader->setVec3(positionName.c_str(), position);
		shader->setVec3(ambientName.c_str(),  ambient);
		shader->setVec3(diffuseName.c_str(),  diffuse);
		shader->setVec3(specularName.c_str(), specular);
		shader->setF32(constantName.c_str(),  constant);
		shader->setF32(linearName.c_str(),    linear);
		shader->setF32(quadraticName.c_str(), quadratic);
	}
};

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

	cam   = new Cam();

	Shader* shader      = new Shader("manylights.vert", "manylights.frag");
	Shader* lampShader  = new Shader("lamp.vert", "lamp.frag");
	Cube*   cubex       = new Cube();
	f32     cutOff      = glm::cos(M_PI / 15.0f);
	f32     outerCutOff = glm::cos(M_PI / 12.0f);

	std::vector<PointLight*> pointLights(4);
	pointLights[0] = new PointLight(shader, "xyzpl[0]", { 0.7f,  0.2f,   2.0f});
	pointLights[1] = new PointLight(shader, "xyzpl[1]", { 2.3f, -3.3f,  -4.0f});
	pointLights[2] = new PointLight(shader, "xyzpl[2]", {-4.0f,  2.0f, -12.0f});
	pointLights[3] = new PointLight(shader, "xyzpl[3]", { 0.0f,  0.0f,  -3.0f});

	auto maze = mkMaze(20);
	std::vector<glm::vec3> positions;
	for (u32 i = 0; i < maze.size(); ++i) {
		for (u32 j = 0; j < maze.size(); ++j) {
			if (maze[i][j] == 0) {
				positions.push_back({i*1.0f, 0.0f, j*1.0f});
			}
		}
	}

	u32 cubeVao, lampVao, vbo, ebo;
	glGenVertexArrays(1, &cubeVao);
	glGenVertexArrays(1, &lampVao);
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

	glBindVertexArray(lampVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(0));
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	shader->loadTexture("container2.png", "xyzmat.diffuse");           // tex0
	shader->loadTexture("container2_specular.png", "xyzmat.specular"); // tex1

	shader->activeTexture(0);
	shader->activeTexture(1);

	auto distance = [](glm::vec3 a, glm::vec3 b) {
		return sqrt(pow(a.x - b.x, 2) + pow(a.z - b.z, 2));
	};

	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, wireframe? GL_LINE: GL_FILL);

		glm::mat4 proj = glm::perspective(cam->getZoom(), ASPECT, 0.1f, 100.0f);

		if (distance(cam->getPos(), glm::vec3(5.0f)) < 2) {
			pointLights[0]->setPosition({5.0f, 1.0f, 5.0f});
		}
		//lightPos.x = 1.5f * (cos(currentFrame) + sin(currentFrame));
		//lightPos.z = 1.5f * (cos(currentFrame) - sin(currentFrame));

		shader->useProgram();

		shader->setVec3("xyz", cam->getPos());

		shader->setVec3("xyzdl.direction", -0.2f, -1.0f, -0.3f);
		shader->setVec3("xyzdl.ambient", 0.05f, 0.05f, 0.05f);
		shader->setVec3("xyzdl.diffuse", 0.4f, 0.4f, 0.4f);
		shader->setVec3("xyzdl.specular", 0.5f, 0.5f, 0.5f);

		shader->setVec3("xyzsl.position", cam->getPos());
		shader->setVec3("xyzsl.direction", cam->getFront());
		shader->setF32("xyzsl.cutOff", cutOff);
		shader->setF32("xyzsl.outerCutOff", outerCutOff);
		shader->setVec3("xyzsl.ambient", 0.2f, 0.2f, 0.2f);
		shader->setVec3("xyzsl.diffuse", 0.5f, 0.5f, 0.5f);
		shader->setVec3("xyzsl.specular", 1.0f, 1.0f, 1.0f);
		shader->setF32("xyzsl.constant", 1.0f);
		shader->setF32("xyzsl.linear", 0.09f);
		shader->setF32("xyzsl.quadratic", 0.032f);

		shader->setF32("xyzmat.shininess", 32.0f);

		for (auto light : pointLights) {
			light->setUniforms();
		}

		glm::mat4 model;
		shader->setMat4("proj", proj);
		shader->setMat4("view", cam->getViewM4());
		glBindVertexArray(cubeVao);

		for (auto pos : positions) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pos);
			shader->setMat4("model", model);
			glDrawElements(GL_TRIANGLES, cubex->getISize(), GL_UNSIGNED_INT, 0);
		}

		// Luz claritaaaaaa ah ahh...
		lampShader->useProgram();
		glBindVertexArray(lampVao);

		lampShader->setMat4("proj", proj);
		lampShader->setMat4("view", cam->getViewM4());

		for (auto light : pointLights) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, light->getPosition());
			model = glm::scale(model, glm::vec3(0.05));
			lampShader->setMat4("model", model);
			glDrawElements(GL_TRIANGLES, cubex->getISize(), GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	};

	glDeleteVertexArrays(1, &cubeVao);
	glDeleteVertexArrays(1, &lampVao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete shader;
	delete lampShader;
	delete cubex;
	delete cam;

	return 0;
}

