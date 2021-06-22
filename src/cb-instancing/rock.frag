#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main() {
	color = texture(texture_diffuse1, texCoords);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

