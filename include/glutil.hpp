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
#include <vector>

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

void framebuffer_size_callback(GLFWwindow*, i32 w, i32 h) {
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

#endif

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

