#version 330 core
in vec3 fragColor;

//uniform vec3 objectColor;
//uniform vec3 lightColor;

out vec4 color;
void main() {
	//color = vec4(lightColor * objectColor, 1.0);
	color = vec4(fragColor, 1.0);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

