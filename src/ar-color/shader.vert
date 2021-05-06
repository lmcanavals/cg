#version 330 core
layout (location = 0) in vec3 vertPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * model * vec4(vertPos, 1.0);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

