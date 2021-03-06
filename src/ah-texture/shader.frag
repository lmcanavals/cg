#version 330 core
in vec3 fragColor;
in vec2 texCoord;

uniform sampler2D texture1;

out vec4 color;
void main() {
    color = texture(texture1, texCoord);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

