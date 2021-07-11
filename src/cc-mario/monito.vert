#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 texCoords;

uniform mat4 view;
uniform mat4 proj;

void main() {
	texCoords = aTexCoords;

	gl_Position = proj * view * aInstanceMatrix * vec4(aPos, 1.0);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

