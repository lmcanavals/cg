#ifndef __GLUTIL_H__
#define __GLUTIL_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>
#include <iostream>
#include <string>

typedef float  f32;
typedef double f64;

typedef char      i8;
typedef short int i16;
typedef int       i32;
typedef long long i64;

typedef unsigned char      ui8;
typedef unsigned short int ui16;
typedef unsigned int       ui32;
typedef unsigned long long ui64;

void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* glutilInit(i32 major, i32 minor,
		i32 width, i32 height,
		const i8* title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
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
	ui32 pid;

	i32 ok;             // check for error status
	i8 infoLog[512];  // get error status info

public:
	Shader(
			std::string shaderPath = "bin",
			std::string vertexFileName = "shader.vert",
			std::string fragmentFileName = "shader.frag") {
		std::ifstream vertexFile(shaderPath + "/" + vertexFileName);
		std::string vertexSrc;
		std::getline(vertexFile, vertexSrc, '\0');

		std::ifstream fragmentFile(shaderPath + "/" + fragmentFileName);
		std::string fragmentSrc;
		std::getline(fragmentFile, fragmentSrc, '\0');

		ui32 vertex = mkShader(vertexSrc.c_str(), GL_VERTEX_SHADER);
		ui32 fragment = mkShader(fragmentSrc.c_str(), GL_FRAGMENT_SHADER);

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
:	ui32 getProgram() { // might need to refactor this later ughhh
		return pid;
	}
	void setMat4(const i8* name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(pid, name, 1, GL_FALSE, &mat[0][0]);
	}

private:
	ui32 mkShader(const i8* source, GLenum type) {
		ui32 shader = glCreateShader(type);
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

