#version 330 core

in vec3 Normal;
in vec3 fragColor;
in vec3 fragPos;

uniform vec3 xyz;          // Posición de la luz
uniform vec3 objectColor;  // color de la luz

out vec4 color;

void main() {
	// ambient
	float strength = 0.1;
	vec3 ambient   = strength * objectColor;

	// diffuse
	vec3  norm           = normalize(Normal);
	vec3  objectColorDir = normalize(xyz - fragPos);
	float diff           = max(dot(norm, objectColorDir), 0.0);
	vec3  diffuse        = diff * objectColor;

	vec3 result    = (ambient + diffuse) * fragColor;
	color          = vec4(result, 1.0);
	// color = vec4(ambient*fragColor, 1.0); // solo ambient light
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

