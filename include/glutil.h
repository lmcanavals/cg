#ifndef __GLUTIL_H__
#define __GLUTIL_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>
#include <path.h>

#include <fstream>
#include <iostream>
#include <string>

typedef              float f32;
typedef             double f64;
typedef               char i8;
typedef          short int i16;
typedef                int i32;
typedef          long long i64;
typedef unsigned      char u8;
typedef unsigned short int u16;
typedef unsigned       int u32;
typedef unsigned long long u64;

void framebuffer_size_callback(GLFWwindow* window, i32 w, i32 h) {
	glViewport(0, 0, w, h);
}

GLFWwindow* glutilInit(i32 major, i32 minor, i32 w, i32 h, const i8* title,
	                     i32 screenWidth=1366, i32 screenHeight=768) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(w, h, title, nullptr, nullptr);
	glfwSetWindowPos(window, screenWidth/2 - w/2, screenHeight/2 - h/2);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW Window\n";
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Not GLAD at all!\n";
		return nullptr;
	}

	return window;
}

class Shader {
	u32 pid;
	Path* path;

	i32 ok;           // check for error status
	i8 infoLog[512];  // get error status info

public:
	Shader(std::string shadersPath = "bin",
		     std::string texturesPath = "resources/textures",
		     std::string vertexFileName = "shader.vert",
		     std::string fragmentFileName = "shader.frag")
			: path(new Path(shadersPath, texturesPath)) {
		std::ifstream vertexFile(path->sp(vertexFileName));
		std::string vertexSrc;
		std::getline(vertexFile, vertexSrc, '\0');

		std::ifstream fragmentFile(path->sp(fragmentFileName));
		std::string fragmentSrc;
		std::getline(fragmentFile, fragmentSrc, '\0');

		u32 vertex = mkShader(vertexSrc.c_str(), GL_VERTEX_SHADER);
		u32 fragment = mkShader(fragmentSrc.c_str(), GL_FRAGMENT_SHADER);

		pid = glCreateProgram();
		glAttachShader(pid, vertex);
		glAttachShader(pid, fragment);
		glLinkProgram(pid);
		glGetProgramiv(pid, GL_LINK_STATUS, &ok);
		if (!ok) {
			glGetProgramInfoLog(pid, 512, nullptr, infoLog);
			std::cout << "Error::shader::program::link_failed\n"
				<< infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	~Shader() {
		glDeleteProgram(pid);
		glfwTerminate();
	}
	void useProgram() {
		glUseProgram(pid);
	}
	u32 getProgram() { // might need to refactor this later ughhh
		return pid;
	}
	// Set uniforms
	void setI32(const i8* name, const i32& i) const {
		glUniform1i(glGetUniformLocation(pid, name), i);
	}
	void setVec3(const i8* name, f32 a, f32 b, f32 c) const {
		glUniform3f(glGetUniformLocation(pid, name), a, b, c);
	}
	void setMat4(const i8* name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(pid, name), 1, GL_FALSE, &mat[0][0]);
	}

	// Texture loading
	u32 loadTexture(const std::string& textureFile,
		              i32 param=GL_LINEAR) {
		u32 texture;
		std::string fileName = path->tp(textureFile);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		
		i32 width, height, nrChannels;

		stbi_set_flip_vertically_on_load(true); // porque en opgl el eje Y invertio
		u8* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
		if (data != nullptr) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
			             GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cerr << "Can't load texture\n";
		}
		stbi_image_free(data);

		return texture;
	}

private:
	u32 mkShader(const i8* source, GLenum type) {
		u32 shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
		if (!ok) {
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "Error::shader::compilation_failed\n"
				<< infoLog << std::endl;
			return 0;
		}
		return shader;
	}
};

#endif

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

