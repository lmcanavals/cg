#version 330 core

uniform vec3 xyzColor;
out vec4 fragColor;

void main() {
	fragColor = vec4(xyzColor, 1.0);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

