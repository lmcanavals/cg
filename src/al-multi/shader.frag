#version 330 core
in vec3 fragColor;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 color;
void main() {
    color = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.3);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

