#include <glutil.h>

const unsigned int FSIZE = sizeof(float);
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Rectangulito");
	Shader* shader = new Shader();

	float vertices[] = {
		 // posiciones         colores       texturas
		 0.5,  0.5,  0.0,   1.0, 0.0, 0.0,   1.0, 1.0,
		 0.5, -0.5,  0.0,   0.0, 1.0, 0.0,   1.0, 0.0,
		-0.5, -0.5,  0.0,   1.0, 0.5, 0.0,   0.0, 0.0,
		-0.5,  0.5,  0.0,   0.0, 0.0, 1.0,   0.0, 1.0 };
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

	// posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)0);
	glEnableVertexAttribArray(0);
	// colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(3*FSIZE));
	glEnableVertexAttribArray(1);
	// coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*FSIZE, (void*)(6*FSIZE));
	glEnableVertexAttribArray(2);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true); // porque en opgl el eje Y invertio
	unsigned char* data = stbi_load(
			"resources/textures/mafalda.jpg", // pruebe con ruta absoluta!
			&width, &height, &nrChannels, 0);
	if (data != nullptr) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		             GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Can't load texture\n";
	}
	stbi_image_free(data);

	glBindVertexArray(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.1, 0.2, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->useProgram();

		glm::mat4 model      = glm::mat4(1.0);
		glm::mat4 view       = glm::mat4(1.0);
		glm::mat4 projection = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
		view  = glm::translate(view, glm::vec3(0.0, 0.0, -6.0));
		projection = glm::perspective(glm::radians(45.0f),
				(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(shader->getProgram(), "model");
		unsigned int viewLoc = glGetUniformLocation(shader->getProgram(), "view");
		unsigned int projLoc = glGetUniformLocation(shader->getProgram(), "proj");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

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
