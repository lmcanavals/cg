#version 330 core

in vec3 Normal;
in vec3 fragColor;
in vec3 fragPos;

uniform vec3 xyz;
uniform vec3 objectColor;

out vec4 color;

void main() {
	// ambient
	float strength = 0.1;
	vec3 ambient   = strength * objectColor;

	// diffuse
	vec3 norm      = normalize(Normal);
	vec3 lightDir  = normalize(xyz - fragPos);
	float diff     = max(dot(norm, lightDir), 0.0);
	vec3 diffuse   = diff * objectColor;

	vec3 result    = (ambient + diffuse) * fragColor;
	color          = vec4(result, 1.0);
}

/* vim: set tabstop=2:softtabstop=2:shiftwidth=2:noexpandtab */

