#include <glutil.h>

const u32 FSIZE = sizeof(f32);

/**
 * keyboard input processing
 **/
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, 960, 540, "Rectangulito");
	Shader* shader = new Shader(); // default: shader.vert and shader.frag

	f32 vertices[] = {
		 // posiciones         colores       texturas
		 0.5,  0.5,  0.0,   1.0, 0.0, 0.0,   1.0, 1.0,
		 0.5, -0.5,  0.0,   0.0, 1.0, 0.0,   1.0, 0.0,
		-0.5, -0.5,  0.0,   1.0, 0.5, 0.0,   0.0, 0.0,
		-0.5,  0.5,  0.0,   0.0, 0.0, 1.0,   0.0, 1.0 };
	u32 idxs[] = {
		0, 1, 3,
		1, 2, 3 };

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

	u32 texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	i32 width, height, nrChannels;

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

		glm::mat4 transform = glm::mat4(1.0); // creates identity matrix
		transform = glm::translate(transform, glm::vec3(0.5, -0.5, 0.0));
		f32 theta = (f32)glfwGetTime();
		transform = glm::rotate(transform, theta, glm::vec3(0.0, 0.0, 1.0));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->useProgram();

		i32 location = glGetUniformLocation(shader->getProgram(), "transform");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));

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

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

