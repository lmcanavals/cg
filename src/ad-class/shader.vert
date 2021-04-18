#version 330 core
layout (location = 0) in vec3 vertPos;
void main() {
    gl_Position = vec4(vertPos, 1.0f);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

